#pragma once
#include <string>

#include "FuelSystem.hpp"
#include "Weather.hpp"

class Aircraft {
public:
    Aircraft(std::string registration, std::string model, double cruiseSpeed, double burnRate,
             FuelSystem fuelSystem);

    std::string getRegistration() const;
    std::string getModel() const;
    double getBurnRate() const;
    FuelSystem& getFuelSystem();
    const FuelSystem& getFuelSystem() const;

    // Fuel to fly a distance (nm) on a heading in this weather.
    double fuelBurn(double distance, const Weather& weather, double heading) const;

    // Furthest distance (nm) on full tanks, no wind, no reserves.
    double maxRange() const;

    // TODO: add engine efficiency, max weight and mid-flight refuelling.

private:
    std::string registration;
    std::string model;
    double cruiseSpeed;  // knots
    double burnRate;     // kg per hour
    FuelSystem fuelSystem;
};
