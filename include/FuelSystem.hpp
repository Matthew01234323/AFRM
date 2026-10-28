#pragma once
#include <vector>

#include "FuelTank.hpp"

class FuelSystem {
public:
    // most aircraft only have one tank, so this is the standard setup
    explicit FuelSystem(double capacity);
    explicit FuelSystem(std::vector<FuelTank> tanks);

    double totalFuel() const;
    double totalCapacity() const;
    bool anyTankLow() const;

    std::vector<FuelTank>& getTanks();
    const std::vector<FuelTank>& getTanks() const;

private:
    std::vector<FuelTank> tanks;
};
