#include "FlightPlan.hpp"

#include "FuelCalculator.hpp"

FlightPlan::FlightPlan(std::string flightNumber, const Aircraft& aircraft, Route route,
                       Weather weather)
    : flightNumber(flightNumber),
      aircraft(aircraft),
      route(route),
      weather(weather),
      plannedFuel(aircraft.getFuelSystem().totalFuel()) {}

double FlightPlan::requiredFuel() const {
    const double trip = FuelCalculator::tripFuel(aircraft, route, weather);
    return trip + FuelCalculator::contingencyFuel(trip) + FuelCalculator::reserveFuel(aircraft);
}

bool FlightPlan::validate() const {
    if (!aircraft.getFuelSystem().checkBalance()) return false;
    if (plannedFuel < requiredFuel() && !aircraft.canRefuel()) return false;
    return true;
}
