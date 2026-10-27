#pragma once

// A single straight leg for now.
// TODO: build the route from waypoints.
class Route {
public:
    Route(double distance, double heading);

    double totalDistance() const;  // nautical miles
    double getHeading() const;     // degrees

private:
    double distance;
    double heading;
};
