#pragma once
#include <string>

// Simulated performance data for one aircraft type.
// Values are made up for the prototype - NOT real aircraft data.
struct Aircraft {
    std::string name;
    double cruiseSpeedKt;     // cruise speed in knots
    double cruiseFlowKgPerH;  // fuel burned per hour in cruise (kg/h)
    double taxiFuelKg;        // fixed fuel allowance for taxi (kg)
    double maxFuelKg;         // tank capacity (kg)
};
