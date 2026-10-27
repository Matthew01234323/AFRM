#pragma once

// A single fuel tank.
// TODO: support several tanks per aircraft (left/right wing).
class FuelTank {
public:
    explicit FuelTank(double capacity);

    bool addFuel(double amount);  // false if it would overfill
    double getLevel() const;
    double getCapacity() const;

private:
    double capacity;  // kg
    double level;     // kg
};
