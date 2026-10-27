#pragma once
#include <string>

#include "FuelTank.hpp"

class Aircraft {
public:
    Aircraft(std::string registration, std::string model, double cruiseSpeed, double burnRate,
             double tankCapacity);

    std::string getRegistration() const;
    std::string getModel() const;
    double getBurnRate() const;
    FuelTank& getTank();
    const FuelTank& getTank() const;

    // Fuel to fly a distance (nm) with a headwind (kt). Negative headwind = tailwind.
    double fuelBurn(double distance, double headwind) const;

private:
    std::string registration;
    std::string model;
    double cruiseSpeed;  // knots
    double burnRate;     // kg per hour
    FuelTank tank;
};
