#include "FuelCalculator.hpp"

FuelCalculator::FuelCalculator(const Aircraft& aircraft) : aircraft_(aircraft) {}

FuelPlan FuelCalculator::calculate(double distanceNm) const {
    FuelPlan plan;

    plan.taxiKg = aircraft_.taxiFuelKg;

    // Simplification: whole trip flown at cruise speed.
    // TODO: split into take-off, climb, cruise, descent and landing phases.
    const double flightTimeH = distanceNm / aircraft_.cruiseSpeedKt;
    plan.tripKg = flightTimeH * aircraft_.cruiseFlowKgPerH;

    // Contingency: 5% of trip fuel.
    plan.contingencyKg = plan.tripKg * 0.05;

    // Final reserve: 30 minutes of flying.
    // TODO: use holding fuel flow instead of cruise fuel flow.
    plan.finalReserveKg = 0.5 * aircraft_.cruiseFlowKgPerH;

    // TODO: add alternate fuel (fuel to reach the alternate airport).

    plan.totalKg = plan.taxiKg + plan.tripKg + plan.contingencyKg + plan.finalReserveKg;
    return plan;
}

bool FuelCalculator::isSufficient(const FuelPlan& plan, double fuelOnBoardKg) const {
    return fuelOnBoardKg >= plan.totalKg && fuelOnBoardKg <= aircraft_.maxFuelKg;
}
