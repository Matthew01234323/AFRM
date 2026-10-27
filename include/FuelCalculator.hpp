#pragma once
#include "Aircraft.hpp"

class FuelCalculator {
public:
    static double tripFuel(const Aircraft& aircraft, double distance, double headwind);
    static double contingencyFuel(double tripFuel);       // 5% of trip fuel
    static double reserveFuel(const Aircraft& aircraft);  // 30 minutes of flying
    static bool isSufficient(const Aircraft& aircraft, double fuelNeeded);
};
