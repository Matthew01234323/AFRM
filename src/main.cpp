// AFRM - Aircraft Fuel and Range Management prototype
// Simulated data only. NOT FOR OPERATIONAL USE.
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "Aircraft.hpp"
#include "FuelCalculator.hpp"

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
    std::cout << "=== AFRM prototype v0.2 (simulated data) ===\n\n";

    // SIMULATED values - not real aircraft data.
    // Aircraft(registration, model, cruise speed kt, burn rate kg/h, tank capacity kg)
    // TODO: move the fleet into its own file.
    std::vector<Aircraft> fleet = {
        Aircraft("SIM-001", "Narrow-body jet", 450, 2500, 18700),
        Aircraft("SIM-002", "Regional turboprop", 275, 650, 5000),
    };

    // --- Choose aircraft ---
    for (std::size_t i = 0; i < fleet.size(); ++i) {
        std::cout << "  " << i + 1 << ". " << fleet[i].getRegistration() << "  "
                  << fleet[i].getModel() << "\n";
    }
    const int choice = static_cast<int>(readNumber("Select aircraft", 1, fleet.size()));
    Aircraft& aircraft = fleet[static_cast<std::size_t>(choice - 1)];

    // --- Inputs ---
    // TODO: replace the single headwind value with proper weather (wind speed and direction).
    const double distance = readNumber("Distance (nm)", 1, 10800);  // half the Earth
    const double headwind = readNumber("Headwind (kt, negative = tailwind)", -200, 200);
    aircraft.getTank().addFuel(
        readNumber("Fuel on board (kg)", 0, aircraft.getTank().getCapacity()));

    // --- Calculate ---
    const double trip = FuelCalculator::tripFuel(aircraft, distance, headwind);
    const double contingency = FuelCalculator::contingencyFuel(trip);
    const double reserve = FuelCalculator::reserveFuel(aircraft);
    const double required = trip + contingency + reserve;

    std::cout << "\n--- " << aircraft.getRegistration() << " ---\n"
              << "Trip fuel:      " << trip << " kg\n"
              << "Contingency:    " << contingency << " kg\n"
              << "Final reserve:  " << reserve << " kg\n"
              << "Required:       " << required << " kg\n"
              << "On board:       " << aircraft.getTank().getLevel() << " kg\n";

    const bool sufficient = FuelCalculator::isSufficient(aircraft, required);
    std::cout << "\nStatus: " << (sufficient ? "SUFFICIENT" : "INSUFFICIENT") << "\n";
    return 0;
}
