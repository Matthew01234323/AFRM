#pragma once
#include <vector>

#include "FuelTank.hpp"

class FuelSystem {
public:
    explicit FuelSystem(std::vector<FuelTank> tanks);

    double totalFuel() const;
    double totalCapacity() const;
    bool transferFuel(int from, int to, double amount);  // tank numbers, e.g. 0 and 1
    bool checkBalance() const;  // all tanks within 10% of each other

    std::vector<FuelTank>& getTanks();

private:
    std::vector<FuelTank> tanks;
};
