// AFRM graph maker
// Reads the test data and the latest test results and writes Mermaid charts.
// Open results/graphs/graphs.md in VS Code (with a Mermaid preview extension)
// or on GitHub to see them, or paste a .mmd file into mermaid.live to export it.
//
// Usage: afrm_graphs [test file] [results folder]
//   defaults: data/test_flights.csv  results
#include <algorithm>
#include <cctype>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Fleet.hpp"
#include "Weather.hpp"

namespace fs = std::filesystem;

const std::string colours = "#2a78d6, #eb6834, #1baf7a, #eda100";

using Row = std::map<std::string, std::string>;

std::vector<std::string> split(const std::string& text, char separator) {
    std::vector<std::string> parts;
    std::stringstream stream(text);
    std::string part;
    while (std::getline(stream, part, separator)) parts.push_back(part);
    if (!text.empty() && text.back() == separator) parts.push_back("");
    return parts;
}

// reads a CSV file into rows, keyed by the column names in the header
std::vector<Row> readCsv(const std::string& path) {
    std::vector<Row> rows;
    std::ifstream file(path);
    std::string line;
    if (!std::getline(file, line)) return rows;
    if (!line.empty() && line.back() == '\r') line.pop_back();
    std::vector<std::string> header = split(line, ',');
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        std::vector<std::string> values = split(line, ',');
        Row row;
        for (std::size_t i = 0; i < header.size() && i < values.size(); i++) {
            row[header[i]] = values[i];
        }
        rows.push_back(row);
    }
    return rows;
}

// finds the newest test_results_<date>.csv (the date in the name sorts correctly)
std::string latestResults(const std::string& folder) {
    std::string latest;
    if (!fs::exists(folder)) return latest;
    for (const auto& entry : fs::directory_iterator(folder)) {
        std::string name = entry.path().filename().string();
        if (name.rfind("test_results_", 0) == 0 && entry.path().extension() == ".csv") {
            if (latest.empty() || entry.path().string() > latest) latest = entry.path().string();
        }
    }
    return latest;
}

// rounds up to a tidy axis maximum, e.g. 1234 -> 1300
double niceMax(double value) {
    double step = std::pow(10, std::floor(std::log10(value)) - 1);
    return std::ceil(value * 1.05 / step) * step;
}

std::string join(const std::vector<std::string>& items) {
    std::string out;
    for (std::size_t i = 0; i < items.size(); i++) out += (i ? ", " : "") + items[i];
    return out;
}

std::string quoted(const std::vector<std::string>& items) {
    std::vector<std::string> q;
    for (const std::string& item : items) q.push_back("\"" + item + "\"");
    return join(q);
}

std::string number(double value, int decimals) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(decimals) << value;
    return out.str();
}

std::string colourHeader(const std::string& palette) {
    return "%%{init: {\"themeVariables\": {\"xyChart\": {\"plotColorPalette\": \"" + palette +
           "\"}}}}%%\n";
}

struct Chart {
    std::string file;
    std::string title;
    std::string caption;
    std::string code;
};

int main(int argc, char* argv[]) {
    std::string testFile = argc > 1 ? argv[1] : "data/test_flights.csv";
    std::string resultsFolder = argc > 2 ? argv[2] : "results";
    std::string resultsFile = latestResults(resultsFolder);
    if (resultsFile.empty()) {
        std::cerr << "No test results found in " << resultsFolder << " - run the tests first.\n";
        return 1;
    }

    std::vector<Row> tests = readCsv(testFile);
    std::vector<Row> results = readCsv(resultsFile);
    if (tests.empty() || results.empty()) {
        std::cerr << "Could not read " << testFile << " or " << resultsFile << "\n";
        return 1;
    }
    std::map<std::string, Row> testById;
    for (const Row& t : tests) testById[t.at("id")] = t;

    std::vector<Aircraft> fleet = loadFleet();
    std::vector<Chart> charts;

    // ---- 1. overall expected results ----
    std::map<std::string, int> statusCount;
    for (const Row& r : results) statusCount[r.at("expected_status")]++;
    charts.push_back({"chart1_overall_results.mmd", "Expected results",
                      "Split of expected results across all test flights.",
                      "%%{init: {\"themeVariables\": {\"pie1\": \"#0ca30c\", \"pie2\": \"#d03b3b\", "
                      "\"pie3\": \"#fab219\"}}}%%\npie showData\n    title Expected results of the " +
                          std::to_string(results.size()) + " test flights\n" +
                          "    \"VALID\" : " + std::to_string(statusCount["VALID"]) + "\n" +
                          "    \"NOT VALID\" : " + std::to_string(statusCount["NOT VALID"]) + "\n" +
                          "    \"REJECTED\" : " + std::to_string(statusCount["REJECTED"]) + "\n"});

    // ---- 2. passed per category ----
    std::vector<std::string> categories;
    std::map<std::string, int> passed, total;
    int allPassed = 0;
    for (const Row& r : results) {
        const std::string& c = r.at("category");
        if (total[c] == 0) categories.push_back(c);
        total[c]++;
        if (r.at("result") == "PASS") {
            passed[c]++;
            allPassed++;
        }
    }
    std::vector<std::string> passedValues;
    int biggest = 0;
    for (const std::string& c : categories) {
        passedValues.push_back(std::to_string(passed[c]));
        biggest = std::max(biggest, total[c]);
    }
    charts.push_back({"chart2_passed_per_category.mmd", "Tests passed per category",
                      "Number of tests passed in each category.",
                      colourHeader("#2a78d6") + "xychart-beta\n    title \"Tests passed per category (" +
                          std::to_string(allPassed) + " / " + std::to_string(results.size()) +
                          " passed)\"\n    x-axis [" + quoted(categories) + "]\n" +
                          "    y-axis \"Tests passed\" 0 --> " + number(niceMax(biggest), 0) + "\n" +
                          "    bar [" + join(passedValues) + "]\n"});

    // ---- 3. rejected inputs by cause ----
    std::map<std::string, int> causes;
    int rejected = 0;
    for (const Row& r : results) {
        if (r.at("actual_status") != "REJECTED") continue;
        rejected++;
        std::string note = r.count("notes") ? r.at("notes") : "";
        std::string cause;
        if (note.find("tank values") != std::string::npos) {
            cause = "Wrong tank count";
        } else if (note.find("whole number") != std::string::npos) {
            cause = "Aircraft (decimal)";
        } else {
            cause = note.substr(0, note.find(" '"));
            if (cause.size() > 5 && cause.substr(cause.size() - 5) == " fuel") cause = "Tank fuel";
            if (!cause.empty()) cause[0] = static_cast<char>(std::toupper(cause[0]));
        }
        causes[cause]++;
    }
    std::vector<std::pair<std::string, int>> sortedCauses(causes.begin(), causes.end());
    std::sort(sortedCauses.begin(), sortedCauses.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    std::vector<std::string> causeNames, causeValues;
    for (const auto& [name, count] : sortedCauses) {
        causeNames.push_back(name);
        causeValues.push_back(std::to_string(count));
    }
    if (!sortedCauses.empty()) {
        charts.push_back({"chart3_rejected_by_cause.mmd", "Rejected inputs by cause",
                          "Which input caused each rejected test.",
                          colourHeader("#2a78d6") + "xychart-beta horizontal\n" +
                              "    title \"Rejected inputs by cause (" + std::to_string(rejected) +
                              " tests)\"\n    x-axis [" + quoted(causeNames) + "]\n" +
                              "    y-axis \"Tests rejected\" 0 --> " +
                              number(niceMax(sortedCauses.front().second), 0) + "\n" +
                              "    bar [" + join(causeValues) + "]\n"});
    }

    // ---- 4. average required fuel by distance band, one line per aircraft ----
    const double band = 500;
    std::map<int, std::map<int, std::vector<double>>> fuelByBand;  // aircraft -> band -> fuel
    for (const Row& r : results) {
        if (r.at("category") != "typical" || r.at("actual_required_kg").empty()) continue;
        const Row& t = testById[r.at("id")];
        int aircraft = std::stoi(t.at("aircraft"));
        int b = static_cast<int>(std::stod(t.at("distance")) / band);
        fuelByBand[aircraft][b].push_back(std::stod(r.at("actual_required_kg")));
    }
    // only use bands that every aircraft has data for, so all lines are the same length
    int bands = 0;
    while (true) {
        bool everyAircraft = true;
        for (std::size_t a = 1; a <= fleet.size(); a++) {
            if (fuelByBand[static_cast<int>(a)][bands].empty()) everyAircraft = false;
        }
        if (!everyAircraft) break;
        bands++;
    }
    if (bands > 0) {
        std::vector<std::string> bandNames;
        for (int b = 0; b < bands; b++) {
            bandNames.push_back(number(b * band, 0) + "-" + number((b + 1) * band - 1, 0));
        }
        std::string lines, key;
        std::vector<std::string> colourNames = {"blue", "orange", "green", "yellow"};
        double highest = 0;
        for (std::size_t a = 1; a <= fleet.size(); a++) {
            std::vector<std::string> averages;
            for (int b = 0; b < bands; b++) {
                const std::vector<double>& values = fuelByBand[static_cast<int>(a)][b];
                double sum = 0;
                for (double v : values) sum += v;
                double tonnes = sum / static_cast<double>(values.size()) / 1000;
                highest = std::max(highest, tonnes);
                averages.push_back(number(tonnes, 1));
            }
            lines += "    line [" + join(averages) + "]\n";
            std::string colour = a <= colourNames.size() ? colourNames[a - 1] : "grey";
            key += (a > 1 ? ", " : "") + colour + " = " + fleet[a - 1].getRegistration() + " " +
                   fleet[a - 1].getModel();
        }
        charts.push_back({"chart4_fuel_vs_distance.mmd", "Required fuel against distance",
                          "Average required fuel in each " + number(band, 0) +
                              " nm band (typical tests). Lines: " + key + ".",
                          colourHeader(colours) + "xychart-beta\n" +
                              "    title \"Average required fuel by distance (typical tests)\"\n" +
                              "    x-axis \"Distance (nm)\" [" + quoted(bandNames) + "]\n" +
                              "    y-axis \"Required fuel (tonnes)\" 0 --> " +
                              number(std::ceil(highest * 1.1), 0) + "\n" + lines});
    }

    // ---- 5. effect of wind on required fuel for the first aircraft ----
    std::map<int, double> fuelByHeadwind;  // headwind (kt) -> required fuel (kg)
    for (const Row& r : results) {
        if (r.at("category") != "wind" || r.at("actual_required_kg").empty()) continue;
        const Row& t = testById[r.at("id")];
        if (t.at("aircraft") != "1") continue;
        Weather weather(std::stod(t.at("wind_speed")), std::stod(t.at("wind_dir")),
                        std::stod(t.at("temperature")));
        int headwind = static_cast<int>(std::lround(weather.headwind(std::stod(t.at("heading")))));
        fuelByHeadwind[headwind] = std::stod(r.at("actual_required_kg"));
    }
    if (!fuelByHeadwind.empty()) {
        std::vector<std::string> winds, fuel;
        double low = 1e9, high = 0;
        for (const auto& [wind, kg] : fuelByHeadwind) {
            winds.push_back(std::to_string(wind));
            fuel.push_back(number(kg, 0));
            low = std::min(low, kg);
            high = std::max(high, kg);
        }
        charts.push_back({"chart5_wind_effect.mmd", "Effect of wind on required fuel",
                          "Required fuel for " + fleet[0].getRegistration() +
                              " in the wind tests, against the headwind component.",
                          colourHeader("#2a78d6") + "xychart-beta\n" +
                              "    title \"Effect of wind on required fuel (" +
                              fleet[0].getRegistration() + ")\"\n" +
                              "    x-axis \"Headwind (kt, negative = tailwind)\" [" + join(winds) +
                              "]\n    y-axis \"Required fuel (kg)\" " +
                              number(std::floor(low / 100) * 100 - 100, 0) + " --> " +
                              number(std::ceil(high / 100) * 100 + 100, 0) + "\n" +
                              "    line [" + join(fuel) + "]\n"});
    }

    // ---- write everything out ----
    std::string outFolder = resultsFolder + "/graphs";
    fs::create_directories(outFolder);
    std::ofstream page(outFolder + "/graphs.md");
    page << "# AFRM test graphs\n\nMade from `" << fs::path(resultsFile).filename().string()
         << "` and `" << testFile << "`.\n\n";
    for (const Chart& chart : charts) {
        std::ofstream(outFolder + "/" + chart.file) << chart.code;
        page << "## " << chart.title << "\n\n```mermaid\n" << chart.code << "```\n\n"
             << chart.caption << "\n\n";
        std::cout << "Wrote " << outFolder << "/" << chart.file << "\n";
    }
    std::cout << "Wrote " << outFolder << "/graphs.md (all charts on one page)\n";
    return 0;
}
