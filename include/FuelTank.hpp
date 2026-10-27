#pragma once
#include <string>

class FuelTank {
public:
    FuelTank(std::string id, double capacity);

    bool addFuel(double amount);     // false if it would overfill
    bool removeFuel(double amount);  // false if there isn't enough fuel
    double getLevel() const;
    bool isLow() const;              // below 10% of capacity

    std::string getId() const;
    double getCapacity() const;

private:
    std::string id;
    double capacity;  // kg
    double level;     // kg
};
