#include "FuelSystem.hpp"

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

bool FuelSystem::transferFuel(int from, int to, double amount) {
    FuelTank& source = tanks.at(static_cast<std::size_t>(from));
    FuelTank& target = tanks.at(static_cast<std::size_t>(to));
    if (amount > source.getLevel() || target.getLevel() + amount > target.getCapacity()) {
        return false;
    }
    source.removeFuel(amount);
    target.addFuel(amount);
    return true;
}

bool FuelSystem::checkBalance() const {
    // Simplification: the fullest and emptiest tank must be within 10% of a tank's capacity.
    double lowest = tanks.front().getLevel();
    double highest = lowest;
    for (const FuelTank& tank : tanks) {
        if (tank.getLevel() < lowest) lowest = tank.getLevel();
        if (tank.getLevel() > highest) highest = tank.getLevel();
    }
    return highest - lowest <= tanks.front().getCapacity() * 0.10;
}

std::vector<FuelTank>& FuelSystem::getTanks() { return tanks; }
