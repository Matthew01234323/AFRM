#include "Fleet.hpp"

// SIMULATED values - not real aircraft data.
// Aircraft(registration, model, cruise speed kt, burn rate kg/h, efficiency,
//          max weight kg, mid-flight refuel, fuel system)
std::vector<Aircraft> loadFleet() {
    return {
        Aircraft("SIM-001", "Narrow-body jet", 450, 2500, 1.0, 78000, false,
                 FuelSystem({FuelTank("LEFT", 9350), FuelTank("RIGHT", 9350)})),
        Aircraft("SIM-002", "Regional turboprop", 275, 650, 1.0, 23000, false,
                 FuelSystem({FuelTank("LEFT", 2500), FuelTank("RIGHT", 2500)})),
        Aircraft("SIM-003", "Military transport", 420, 3200, 0.95, 140000, true,
                 FuelSystem({FuelTank("LEFT", 14000), FuelTank("RIGHT", 14000)})),
    };
}
