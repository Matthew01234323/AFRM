// AFRM - Aircraft Fuel and Range Management prototype
// Simulated data only. NOT FOR OPERATIONAL USE.
#include <iomanip>
#include <iostream>
#include <limits>

#include "Aircraft.hpp"
#include "FuelCalculator.hpp"

// Asks the user for a positive number, repeating until one is entered.
double readPositive(const char* prompt) {
    double value = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value > 0) {
            return value;
        }
        std::cout << "  Please enter a number greater than 0.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main() {
    std::cout << "=== AFRM prototype v0.1 (simulated data) ===\n\n";

    // TODO: load aircraft from a data file instead of hard-coding.
    const Aircraft aircraft{"SIM-NB (simulated narrow-body)", 450.0, 2500.0, 150.0, 18700.0};
    std::cout << "Aircraft: " << aircraft.name << "\n\n";

    const double distanceNm = readPositive("Trip distance (nm): ");
    const double fuelOnBoardKg = readPositive("Fuel on board (kg): ");

    const FuelCalculator calculator(aircraft);
    const FuelPlan plan = calculator.calculate(distanceNm);

    std::cout << std::fixed << std::setprecision(0);
    std::cout << "\n--- Fuel plan ---\n"
              << "Taxi:          " << plan.taxiKg << " kg\n"
              << "Trip:          " << plan.tripKg << " kg\n"
              << "Contingency:   " << plan.contingencyKg << " kg\n"
              << "Final reserve: " << plan.finalReserveKg << " kg\n"
              << "Total needed:  " << plan.totalKg << " kg\n"
              << "On board:      " << fuelOnBoardKg << " kg\n\n";

    if (calculator.isSufficient(plan, fuelOnBoardKg)) {
        std::cout << "Status: SUFFICIENT\n";
    } else {
        std::cout << "Status: INSUFFICIENT\n";
    }
    return 0;
}
