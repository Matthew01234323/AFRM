#include "Route.hpp"

Route::Route(double distance, double heading) : distance(distance), heading(heading) {}

double Route::totalDistance() const { return distance; }
double Route::getHeading() const { return heading; }
