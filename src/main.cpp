// AFRM - Aircraft Fuel and Range Management prototype
// Simulated data only, NOT for operational use.
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "Fleet.hpp"
#include "FlightPlan.hpp"
#include "FuelCalculator.hpp"
#include "InputLimits.hpp"

double readNumber(const std::string& prompt, double min, double max) {
    double value;
    while (true) {
        std::cout << prompt << " (" << min << " - " << max << "): ";
        if (std::cin >> value && inRange(value, min, max)) return value;
        std::cout << "  Please enter a number between " << min << " and " << max << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// same as readNumber but doesn't allow decimals (used for the menu)
int readWholeNumber(const std::string& prompt, int min, int max) {
    while (true) {
        double value = readNumber(prompt, min, max);
        if (value == static_cast<int>(value)) return static_cast<int>(value);
        std::cout << "  Please enter a whole number.\n";
    }
}

int main() {
    std::cout << std::fixed << std::setprecision(0);
    std::cout << "=== AFRM prototype v0.6 (simulated data) ===\n\n";

    std::vector<Aircraft> fleet = loadFleet();
    for (std::size_t i = 0; i < fleet.size(); i++) {
        std::cout << "  " << i + 1 << ". " << fleet[i].getRegistration() << "  "
                  << fleet[i].getModel() << "\n";
    }
    int choice = readWholeNumber("Select aircraft", 1, static_cast<int>(fleet.size()));
    Aircraft& aircraft = fleet[static_cast<std::size_t>(choice - 1)];

    double distance = readNumber("Distance (nm)", limits::minDistance, limits::maxDistance);
    double heading = readNumber("Heading (deg)", limits::minHeading, limits::maxHeading);
    double windSpeed = readNumber("Wind speed (kt)", limits::minWindSpeed, limits::maxWindSpeed);
    double windDirection =
        readNumber("Wind from (deg)", limits::minWindDirection, limits::maxWindDirection);
    double temperature =
        readNumber("Temperature (C)", limits::minTemperature, limits::maxTemperature);

    FuelSystem& fuel = aircraft.getFuelSystem();
    for (FuelTank& tank : fuel.getTanks()) {
        tank.addFuel(readNumber("Fuel in " + tank.getId() + " tank (kg)", 0, tank.getCapacity()));
    }

    Route route(distance, heading);
    Weather weather(windSpeed, windDirection, temperature);
    FlightPlan plan(aircraft, route, weather);

    double trip = FuelCalculator::tripFuel(aircraft, route, weather);
    double required = plan.requiredFuel();
    double onBoard = fuel.totalFuel();

    std::cout << "\n--- " << aircraft.getRegistration() << " " << aircraft.getModel() << " ---\n"
              << "Headwind:       " << weather.headwind(heading) << " kt\n"
              << "Trip fuel:      " << trip << " kg\n"
              << "Contingency:    " << FuelCalculator::contingencyFuel(trip) << " kg\n"
              << "Final reserve:  " << FuelCalculator::reserveFuel(aircraft) << " kg\n"
              << "Required:       " << required << " kg\n"
              << "On board:       " << onBoard << " kg\n"
              << "Max range:      " << FuelCalculator::maxRange(aircraft) << " nm (with reserves)\n";

    for (const FuelTank& tank : fuel.getTanks()) {
        if (tank.isLow()) std::cout << "WARNING: " << tank.getId() << " tank is low\n";
    }

    if (FuelCalculator::needsRefuel(aircraft, required)) {
        if (aircraft.canRefuel()) {
            std::cout << "Mid-flight refuel needed: " << required - onBoard << " kg\n";
        } else {
            std::cout << "Not enough fuel - short by " << required - onBoard << " kg\n";
        }
    }
    std::cout << "\nFlight plan: " << (plan.validate() ? "VALID" : "NOT VALID") << "\n";
    return 0;
}
