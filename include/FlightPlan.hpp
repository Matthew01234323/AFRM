#pragma once
#include <string>

#include "Aircraft.hpp"
#include "Route.hpp"
#include "Weather.hpp"

class FlightPlan {
public:
    FlightPlan(std::string flightNumber, const Aircraft& aircraft, Route route, Weather weather);

    double requiredFuel() const;  // trip + contingency + reserve
    bool validate() const;        // tanks balanced and enough fuel (or can refuel in flight)

private:
    std::string flightNumber;
    const Aircraft& aircraft;
    Route route;
    Weather weather;
    double plannedFuel;  // fuel loaded when the plan was made
};
