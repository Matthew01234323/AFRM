#include "Aircraft.hpp"

Aircraft::Aircraft(std::string registration, std::string model, double cruiseSpeed,
                   double burnRate, FuelSystem fuelSystem)
    : registration(registration),
      model(model),
      cruiseSpeed(cruiseSpeed),
      burnRate(burnRate),
      fuelSystem(fuelSystem) {}

std::string Aircraft::getRegistration() const { return registration; }
std::string Aircraft::getModel() const { return model; }
double Aircraft::getBurnRate() const { return burnRate; }
FuelSystem& Aircraft::getFuelSystem() { return fuelSystem; }
const FuelSystem& Aircraft::getFuelSystem() const { return fuelSystem; }

double Aircraft::fuelBurn(double distance, const Weather& weather, double heading) const {
    const double groundSpeed = cruiseSpeed - weather.headwind(heading);
    const double hours = distance / groundSpeed;
    return hours * burnRate;
}

double Aircraft::maxRange() const {
    const double hours = fuelSystem.totalCapacity() / burnRate;
    return hours * cruiseSpeed;
}
