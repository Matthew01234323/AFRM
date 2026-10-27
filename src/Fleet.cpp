#include "Fleet.hpp"

// SIMULATED values - not real aircraft data.
// Aircraft(registration, model, cruise speed kt, burn rate kg/h, fuel system)
std::vector<Aircraft> loadFleet() {
    return {
        Aircraft("SIM-001", "Narrow-body jet", 450, 2500,
                 FuelSystem({FuelTank("LEFT", 9350), FuelTank("RIGHT", 9350)})),
        Aircraft("SIM-002", "Regional turboprop", 275, 650,
                 FuelSystem({FuelTank("LEFT", 2500), FuelTank("RIGHT", 2500)})),
    };
}
