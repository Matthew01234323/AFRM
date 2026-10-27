#include "FuelCalculator.hpp"

double FuelCalculator::tripFuel(const Aircraft& aircraft, const Route& route,
                                const Weather& weather) {
    return aircraft.fuelBurn(route.totalDistance(), weather, route.getHeading());
}

double FuelCalculator::contingencyFuel(double tripFuel) { return tripFuel * 0.05; }

double FuelCalculator::reserveFuel(const Aircraft& aircraft) {
    return aircraft.getBurnRate() * 0.5;
}

double FuelCalculator::maxRange(const Aircraft& aircraft) {
    // Fuel available for the trip once reserves are kept back.
    const double usable = aircraft.getFuelSystem().totalCapacity() - reserveFuel(aircraft);
    const double trip = usable / 1.05;  // trip + 5% contingency must fit in usable fuel
    // Scale the aircraft's full-tank range by the share of fuel available for the trip.
    return aircraft.maxRange() * (trip / aircraft.getFuelSystem().totalCapacity());
}

bool FuelCalculator::isSufficient(const Aircraft& aircraft, double fuelNeeded) {
    return aircraft.getFuelSystem().totalFuel() >= fuelNeeded;
}
