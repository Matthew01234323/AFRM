#include "Aircraft.hpp"

Aircraft::Aircraft(std::string registration, std::string model, double cruiseSpeed,
                   double burnRate, double tankCapacity)
    : registration(registration),
      model(model),
      cruiseSpeed(cruiseSpeed),
      burnRate(burnRate),
      tank(tankCapacity) {}

std::string Aircraft::getRegistration() const { return registration; }
std::string Aircraft::getModel() const { return model; }
double Aircraft::getBurnRate() const { return burnRate; }
FuelTank& Aircraft::getTank() { return tank; }
const FuelTank& Aircraft::getTank() const { return tank; }

double Aircraft::fuelBurn(double distance, const Weather& weather, double heading) const {
    const double groundSpeed = cruiseSpeed - weather.headwind(heading);
    const double hours = distance / groundSpeed;
    return hours * burnRate;
}
