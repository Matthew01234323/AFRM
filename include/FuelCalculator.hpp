#pragma once
#include "Aircraft.hpp"

// Fuel needed for a flight, split into parts.
struct FuelPlan {
    double taxiKg = 0;
    double tripKg = 0;
    double contingencyKg = 0;
    double finalReserveKg = 0;
    double totalKg = 0;
};

class FuelCalculator {
public:
    explicit FuelCalculator(const Aircraft& aircraft);

    // Works out the fuel needed to fly distanceNm (nautical miles).
    FuelPlan calculate(double distanceNm) const;

    // True if the fuel on board covers the fuel plan and fits in the tanks.
    bool isSufficient(const FuelPlan& plan, double fuelOnBoardKg) const;

private:
    Aircraft aircraft_;
};
