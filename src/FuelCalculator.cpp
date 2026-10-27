#include "FuelCalculator.hpp"

double FuelCalculator::tripFuel(const Aircraft& aircraft, const Route& route,
                                const Weather& weather) {
    return aircraft.fuelBurn(route.totalDistance(), weather, route.getHeading());
}

double FuelCalculator::contingencyFuel(double tripFuel) { return tripFuel * 0.05; }

double FuelCalculator::reserveFuel(const Aircraft& aircraft) {
    return aircraft.getBurnRate() * 0.5;
}

bool FuelCalculator::isSufficient(const Aircraft& aircraft, double fuelNeeded) {
    return aircraft.getTank().getLevel() >= fuelNeeded;
}
