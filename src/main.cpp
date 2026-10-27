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
    std::cout << "=== AFRM prototype v0.3 (simulated data) ===\n\n";

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
    aircraft.getTank().addFuel(
        readNumber("Fuel on board (kg)", 0, aircraft.getTank().getCapacity()));

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
              << "On board:       " << aircraft.getTank().getLevel() << " kg\n";

    const bool sufficient = FuelCalculator::isSufficient(aircraft, required);
    std::cout << "\nStatus: " << (sufficient ? "SUFFICIENT" : "INSUFFICIENT") << "\n";
    return 0;
}
