#include "Fleet.hpp"

// made up values for testing, not real aircraft
// Aircraft(registration, model, cruise speed kt, burn rate kg/h, efficiency,
//          max weight kg, mid-flight refuel, fuel system)
std::vector<Aircraft> loadFleet() {
    return {
        Aircraft("SIM-001", "Narrow-body jet", 450, 2500, 1.0, 78000, false,
                 FuelSystem({FuelTank("LEFT", 4600), FuelTank("CENTRE", 9500),
                             FuelTank("RIGHT", 4600)})),
        Aircraft("SIM-002", "Regional turboprop", 275, 650, 1.0, 23000, false,
                 FuelSystem(5000)),
        Aircraft("SIM-003", "Military transport", 420, 3200, 0.95, 140000, true,
                 FuelSystem({FuelTank("LEFT OUTER", 6000), FuelTank("LEFT INNER", 8000),
                             FuelTank("RIGHT INNER", 8000), FuelTank("RIGHT OUTER", 6000)})),
        Aircraft("SIM-004", "Business jet", 430, 900, 1.05, 18000, false, FuelSystem(6000)),
    };
}
