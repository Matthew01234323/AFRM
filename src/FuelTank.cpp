#include "FuelTank.hpp"

FuelTank::FuelTank(double capacity) : capacity(capacity), level(0) {}

bool FuelTank::addFuel(double amount) {
    if (amount < 0 || level + amount > capacity) return false;
    level += amount;
    return true;
}

double FuelTank::getLevel() const { return level; }
double FuelTank::getCapacity() const { return capacity; }
