#include "FuelSystem.hpp"

FuelSystem::FuelSystem(double capacity) : tanks({FuelTank("MAIN", capacity)}) {}

FuelSystem::FuelSystem(std::vector<FuelTank> tanks) : tanks(tanks) {}

double FuelSystem::totalFuel() const {
    double total = 0;
    for (const FuelTank& tank : tanks) total += tank.getLevel();
    return total;
}

double FuelSystem::totalCapacity() const {
    double total = 0;
    for (const FuelTank& tank : tanks) total += tank.getCapacity();
    return total;
}

bool FuelSystem::anyTankLow() const {
    for (const FuelTank& tank : tanks) {
        if (tank.isLow()) return true;
    }
    return false;
}

std::vector<FuelTank>& FuelSystem::getTanks() { return tanks; }
const std::vector<FuelTank>& FuelSystem::getTanks() const { return tanks; }
