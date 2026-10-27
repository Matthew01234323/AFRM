#pragma once

class Weather {
public:
    Weather(double windSpeed, double windDirection, double temperature);

    // Wind along the heading in knots: positive = headwind, negative = tailwind.
    double headwind(double heading) const;

private:
    double windSpeed;      // knots
    double windDirection;  // degrees the wind blows FROM
    double temperature;    // °C - TODO: not used in calculations yet
};
