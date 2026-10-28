#pragma once
#include "Aircraft.hpp"
#include "Route.hpp"
#include "Weather.hpp"

class FlightPlan {
public:
    FlightPlan(const Aircraft& aircraft, Route route, Weather weather);

    double requiredFuel() const;  // trip + contingency + reserve
    bool validate() const;        // enough fuel, or the aircraft can refuel in the air

private:
    const Aircraft& aircraft;
    Route route;
    Weather weather;
    double plannedFuel;  // fuel loaded when the plan was made
};
