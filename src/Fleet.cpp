#include "Fleet.hpp"

// SIMULATED values - not real aircraft data.
// Aircraft(registration, model, cruise speed kt, burn rate kg/h, tank capacity kg)
std::vector<Aircraft> loadFleet() {
    return {
        Aircraft("SIM-001", "Narrow-body jet", 450, 2500, 18700),
        Aircraft("SIM-002", "Regional turboprop", 275, 650, 5000),
    };
}
