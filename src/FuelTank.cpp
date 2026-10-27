#include "FuelTank.hpp"

FuelTank::FuelTank(std::string id, double capacity) : id(id), capacity(capacity), level(0) {}

bool FuelTank::addFuel(double amount) {
    if (amount < 0 || level + amount > capacity) return false;
    level += amount;
    return true;
}

bool FuelTank::removeFuel(double amount) {
    if (amount < 0 || amount > level) return false;
    level -= amount;
    return true;
}

double FuelTank::getLevel() const { return level; }
bool FuelTank::isLow() const { return level < capacity * 0.10; }
std::string FuelTank::getId() const { return id; }
double FuelTank::getCapacity() const { return capacity; }
