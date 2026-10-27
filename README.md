# AFRM - Aircraft Fuel and Range Management (prototype)

Command-line prototype that calculates the fuel needed for a flight and
checks whether the flight plan is valid.

**Simulated data only - not for operational use.**

## Build and run

```
cmake -B build -G Ninja
cmake --build build
.\build\afrm.exe
```

## Status (v0.5)

- Aircraft now has engine efficiency, max weight and mid-flight refuelling
- `FlightPlan` class brings aircraft, route and weather together and validates the plan
- Refuel check: reports mid-flight refuel needed, or fuel shortfall
- Third simulated aircraft (military transport, can refuel in flight)

## Structure

| Class | Purpose |
|---|---|
| `Aircraft` | Aircraft data, its fuel system, fuel burn and range |
| `FuelSystem` | The aircraft's tanks: total fuel, transfers, balance |
| `FuelTank` | One tank: capacity and fuel level |
| `FlightPlan` | Brings aircraft, route and weather together; required fuel and validation |
| `Route` | Distance and heading (single leg for now) |
| `Weather` | Wind and temperature; headwind component |
| `FuelCalculator` | Trip, contingency (5%) and reserve (30 min) fuel, range, refuel check |

New aircraft are added in `src/Fleet.cpp`.

## Planned next
- Routes built from waypoints
- Use temperature and aircraft weight in fuel calculations
- Unit tests (GoogleTest)
