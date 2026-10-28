#pragma once

// Allowed ranges for user inputs. Used by both the main program and the tests
// so they always agree. Fuel per tank is limited by each tank's own capacity.
namespace limits {
const double minDistance = 1;
const double maxDistance = 10800;  // roughly half way round the Earth
const double minHeading = 0;
const double maxHeading = 360;
const double minWindSpeed = 0;
const double maxWindSpeed = 200;
const double minWindDirection = 0;
const double maxWindDirection = 360;
const double minTemperature = -60;
const double maxTemperature = 50;
}  // namespace limits

inline bool inRange(double value, double min, double max) {
    return value >= min && value <= max;
}
