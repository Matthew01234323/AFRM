// AFRM - Aircraft Fuel and Range Management prototype
// Simulated data only. NOT FOR OPERATIONAL USE.
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "Fleet.hpp"
#include "FuelCalculator.hpp"
#include "Route.hpp"
#include "Weather.hpp"

// Asks for a number between min and max, repeating until one is entered.
double readNumber(const std::string& prompt, double min, double max) {
    double value;
    while (true) {
        std::cout << prompt << " (" << min << " - " << max << "): ";
        if (std::cin >> value && value >= min && value <= max) return value;
        std::cout << "  Please enter a number between " << min << " and " << max << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main() {
    std::cout << std::fixed << std::setprecision(0);
    std::cout << "=== AFRM prototype v0.4 (simulated data) ===\n\n";

    std::vector<Aircraft> fleet = loadFleet();

    // --- Choose aircraft ---
    for (std::size_t i = 0; i < fleet.size(); ++i) {
        std::cout << "  " << i + 1 << ". " << fleet[i].getRegistration() << "  "
                  << fleet[i].getModel() << "\n";
    }
    const int choice = static_cast<int>(readNumber("Select aircraft", 1, fleet.size()));
    Aircraft& aircraft = fleet[static_cast<std::size_t>(choice - 1)];

    // --- Inputs ---
    const double distance = readNumber("Distance (nm)", 1, 10800);  // half the Earth
    const double heading = readNumber("Heading (deg)", 0, 360);
    const double windSpeed = readNumber("Wind speed (kt)", 0, 200);
    const double windDirection = readNumber("Wind from (deg)", 0, 360);
    const double temperature = readNumber("Temperature (C)", -60, 50);

    // --- Load fuel into each tank ---
    for (FuelTank& tank : aircraft.getFuelSystem().getTanks()) {
        tank.addFuel(readNumber("Fuel in " + tank.getId() + " tank (kg)", 0, tank.getCapacity()));
    }

    // Balance the tanks if needed by moving half the difference.
    FuelSystem& fuel = aircraft.getFuelSystem();
    if (!fuel.checkBalance()) {
        const double left = fuel.getTanks()[0].getLevel();
        const double right = fuel.getTanks()[1].getLevel();
        if (left > right) fuel.transferFuel(0, 1, (left - right) / 2);
        else fuel.transferFuel(1, 0, (right - left) / 2);
        std::cout << "Tanks were unbalanced - fuel transferred to balance them.\n";
    }

    // --- Calculate ---
    const Route route(distance, heading);
    const Weather weather(windSpeed, windDirection, temperature);
    const double trip = FuelCalculator::tripFuel(aircraft, route, weather);
    const double contingency = FuelCalculator::contingencyFuel(trip);
    const double reserve = FuelCalculator::reserveFuel(aircraft);
    const double required = trip + contingency + reserve;

    std::cout << "\n--- " << aircraft.getRegistration() << " ---\n"
              << "Headwind:       " << weather.headwind(heading) << " kt\n"
              << "Trip fuel:      " << trip << " kg\n"
              << "Contingency:    " << contingency << " kg\n"
              << "Final reserve:  " << reserve << " kg\n"
              << "Required:       " << required << " kg\n"
              << "On board:       " << fuel.totalFuel() << " kg\n"
              << "Max range:      " << FuelCalculator::maxRange(aircraft) << " nm (with reserves)\n";

    for (const FuelTank& tank : fuel.getTanks()) {
        if (tank.isLow()) std::cout << "WARNING: " << tank.getId() << " tank is low\n";
    }

    const bool sufficient = FuelCalculator::isSufficient(aircraft, required);
    std::cout << "\nStatus: " << (sufficient ? "SUFFICIENT" : "INSUFFICIENT") << "\n";
    return 0;
}
