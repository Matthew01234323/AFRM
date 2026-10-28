#include "FlightPlan.hpp"

#include "FuelCalculator.hpp"

FlightPlan::FlightPlan(const Aircraft& aircraft, Route route, Weather weather)
    : aircraft(aircraft),
      route(route),
      weather(weather),
      plannedFuel(aircraft.getFuelSystem().totalFuel()) {}

double FlightPlan::requiredFuel() const {
    double trip = FuelCalculator::tripFuel(aircraft, route, weather);
    return trip + FuelCalculator::contingencyFuel(trip) + FuelCalculator::reserveFuel(aircraft);
}

bool FlightPlan::validate() const {
    if (plannedFuel >= requiredFuel()) return true;
    return aircraft.canRefuel();
}
