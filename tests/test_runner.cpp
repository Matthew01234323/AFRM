// AFRM automated test runner
// Reads a CSV of sample flights, runs each one through the same classes the
// main program uses, and compares the results with the expected values.
//
// Usage: afrm_tests [test file] [results folder]
//   defaults: data/test_flights.csv  results
#include <cmath>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Fleet.hpp"
#include "FlightPlan.hpp"
#include "InputLimits.hpp"

// how close the calculated fuel has to be to the expected value (kg)
const double tolerance = 0.01;

struct TestCase {
    std::string id;
    std::string category;
    std::string aircraft;
    std::string distance;
    std::string heading;
    std::string windSpeed;
    std::string windDirection;
    std::string temperature;
    std::string tankFuel;  // one value per tank, separated by ';'
    std::string expectedRequired;
    std::string expectedLow;
    std::string expectedStatus;
};

struct TestResult {
    std::string status;   // VALID, NOT VALID or REJECTED
    double required = 0;  // only set if not rejected
    bool low = false;
    std::string reason;   // why an input was rejected
};

std::vector<std::string> split(const std::string& text, char separator) {
    std::vector<std::string> parts;
    std::stringstream stream(text);
    std::string part;
    while (std::getline(stream, part, separator)) parts.push_back(part);
    // getline drops a trailing empty field, e.g. "1,2," - put it back
    if (!text.empty() && text.back() == separator) parts.push_back("");
    return parts;
}

// turns text into a number, returns false if it isn't a proper number
bool toNumber(const std::string& text, double& value) {
    try {
        std::size_t used = 0;
        value = std::stod(text, &used);
        return used == text.size();
    } catch (...) {
        return false;
    }
}

// checks one input the same way the main program would
bool checkInput(const std::string& name, const std::string& text, double min, double max,
                double& value, std::string& reason) {
    if (!toNumber(text, value) || !inRange(value, min, max)) {
        reason = name + " '" + text + "' not accepted";
        return false;
    }
    return true;
}

TestResult runTest(const TestCase& test) {
    TestResult result;
    result.status = "REJECTED";

    std::vector<Aircraft> fleet = loadFleet();  // fresh fleet each time so tanks start empty

    double choice;
    if (!checkInput("aircraft", test.aircraft, 1, static_cast<double>(fleet.size()), choice,
                    result.reason)) {
        return result;
    }
    if (choice != static_cast<int>(choice)) {
        result.reason = "aircraft must be a whole number";
        return result;
    }
    Aircraft& aircraft = fleet[static_cast<std::size_t>(choice) - 1];

    double distance, heading, windSpeed, windDirection, temperature;
    if (!checkInput("distance", test.distance, limits::minDistance, limits::maxDistance,
                    distance, result.reason) ||
        !checkInput("heading", test.heading, limits::minHeading, limits::maxHeading, heading,
                    result.reason) ||
        !checkInput("wind speed", test.windSpeed, limits::minWindSpeed, limits::maxWindSpeed,
                    windSpeed, result.reason) ||
        !checkInput("wind direction", test.windDirection, limits::minWindDirection,
                    limits::maxWindDirection, windDirection, result.reason) ||
        !checkInput("temperature", test.temperature, limits::minTemperature,
                    limits::maxTemperature, temperature, result.reason)) {
        return result;
    }

    std::vector<FuelTank>& tanks = aircraft.getFuelSystem().getTanks();
    std::vector<std::string> fuelValues = split(test.tankFuel, ';');
    if (fuelValues.size() != tanks.size()) {
        result.reason = "expected " + std::to_string(tanks.size()) + " tank values, got " +
                        std::to_string(fuelValues.size());
        return result;
    }
    for (std::size_t i = 0; i < tanks.size(); i++) {
        double amount;
        if (!checkInput(tanks[i].getId() + " fuel", fuelValues[i], 0, tanks[i].getCapacity(),
                        amount, result.reason)) {
            return result;
        }
        tanks[i].addFuel(amount);
    }

    FlightPlan plan(aircraft, Route(distance, heading),
                    Weather(windSpeed, windDirection, temperature));
    result.required = plan.requiredFuel();
    result.low = aircraft.getFuelSystem().anyTankLow();
    result.status = plan.validate() ? "VALID" : "NOT VALID";
    return result;
}

std::string timestamp(const char* format) {
    std::time_t now = std::time(nullptr);
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), format, std::localtime(&now));
    return buffer;
}

int main(int argc, char* argv[]) {
    std::string testFile = argc > 1 ? argv[1] : "data/test_flights.csv";
    std::string resultsFolder = argc > 2 ? argv[2] : "results";

    std::ifstream input(testFile);
    if (!input) {
        std::cerr << "Could not open " << testFile << "\n";
        return 1;
    }

    std::filesystem::create_directories(resultsFolder);
    std::string resultsFile = resultsFolder + "/test_results_" + timestamp("%Y-%m-%d_%H-%M-%S") + ".csv";
    std::ofstream output(resultsFile);
    output << "id,category,expected_status,actual_status,expected_required_kg,"
              "actual_required_kg,expected_low_tank,actual_low_tank,result,notes\n";
    output << std::fixed << std::setprecision(2);

    int total = 0, passed = 0;
    std::map<std::string, std::pair<int, int>> byCategory;  // category -> (passed, total)

    std::string line;
    std::getline(input, line);  // skip header
    while (std::getline(input, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();  // Windows line endings
        if (line.empty()) continue;

        std::vector<std::string> f = split(line, ',');
        if (f.size() != 12) {
            std::cerr << "Skipping badly formatted line: " << line << "\n";
            continue;
        }
        TestCase test{f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7], f[8], f[9], f[10], f[11]};
        TestResult actual = runTest(test);

        // compare with what was expected
        bool pass = actual.status == test.expectedStatus;
        std::string notes = actual.reason;
        if (actual.status != "REJECTED") {
            double expected = 0;
            toNumber(test.expectedRequired, expected);
            if (std::fabs(actual.required - expected) > tolerance) {
                pass = false;
                notes = "required fuel differs";
            }
            if ((actual.low ? "yes" : "no") != test.expectedLow) {
                pass = false;
                notes = "low tank warning differs";
            }
        }

        total++;
        byCategory[test.category].second++;
        if (pass) {
            passed++;
            byCategory[test.category].first++;
        }

        output << test.id << "," << test.category << "," << test.expectedStatus << ","
               << actual.status << "," << test.expectedRequired << ",";
        if (actual.status != "REJECTED") output << actual.required;
        output << "," << test.expectedLow << ",";
        if (actual.status != "REJECTED") output << (actual.low ? "yes" : "no");
        output << "," << (pass ? "PASS" : "FAIL") << "," << notes << "\n";
    }

    // summary on screen
    std::cout << "AFRM automated tests\n";
    std::cout << "Test file: " << testFile << "\n\n";
    for (const auto& [category, counts] : byCategory) {
        std::cout << "  " << std::left << std::setw(16) << category << counts.first << " / "
                  << counts.second << " passed\n";
    }
    std::cout << "\nTotal: " << passed << " / " << total << " passed, " << total - passed
              << " failed\n";
    std::cout << "Results saved to " << resultsFile << "\n";

    // keep a running history of every test run
    std::string historyFile = resultsFolder + "/test_history.csv";
    bool newHistory = !std::filesystem::exists(historyFile);
    std::ofstream history(historyFile, std::ios::app);
    if (newHistory) history << "date,test_file,total,passed,failed\n";
    history << timestamp("%Y-%m-%d %H:%M:%S") << "," << testFile << "," << total << ","
            << passed << "," << total - passed << "\n";

    return passed == total ? 0 : 1;
}
