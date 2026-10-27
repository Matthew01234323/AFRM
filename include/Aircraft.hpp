#pragma once
#include <string>

#include "FuelSystem.hpp"
#include "Weather.hpp"

class Aircraft {
public:
    Aircraft(std::string registration, std::string model, double cruiseSpeed, double burnRate,
             double efficiency, double maxWeight, bool midFlightRefuel, FuelSystem fuelSystem);

    std::string getRegistration() const;
    std::string getModel() const;
    FuelSystem& getFuelSystem();
    const FuelSystem& getFuelSystem() const;
    double getEfficiency() const;
    double getBurnRate() const;
    bool canRefuel() const;

    // Fuel to fly a distance (nm) on a heading in this weather, before efficiency.
    double fuelBurn(double distance, const Weather& weather, double heading) const;

    // Furthest distance (nm) on full tanks, no wind, no reserves.
    double maxRange() const;

private:
    std::string registration;
    std::string model;
    double cruiseSpeed;    // knots
    double burnRate;       // kg per hour
    double efficiency;     // 1.0 = normal, 0.9 = burns more fuel, 1.1 = burns less
    double maxWeight;      // kg - TODO: not used in calculations yet
    bool midFlightRefuel;  // can it refuel in the air?
    FuelSystem fuelSystem;
};
