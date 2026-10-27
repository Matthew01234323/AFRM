#pragma once
#include "Aircraft.hpp"
#include "Route.hpp"
#include "Weather.hpp"

class FuelCalculator {
public:
    static double tripFuel(const Aircraft& aircraft, const Route& route, const Weather& weather);
    static double reserveFuel(const Aircraft& aircraft);  // 30 minutes of flying
    static double contingencyFuel(double tripFuel);       // 5% of trip fuel
    static double maxRange(const Aircraft& aircraft);     // full tanks, keeping reserves
    static double applyEfficiency(double burn, double efficiency);
    static bool needsRefuel(const Aircraft& aircraft, double fuelNeeded);
};
