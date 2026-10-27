#include "Weather.hpp"

#include <cmath>

Weather::Weather(double windSpeed, double windDirection, double temperature)
    : windSpeed(windSpeed), windDirection(windDirection), temperature(temperature) {}

double Weather::headwind(double heading) const {
    const double degToRad = 3.14159265358979 / 180.0;
    return windSpeed * std::cos((windDirection - heading) * degToRad);
}
