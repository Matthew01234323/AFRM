# AFRM - Aircraft Fuel and Range Management (prototype)

Command-line prototype that calculates the fuel needed for a flight and
checks whether the fuel on board is sufficient.

**Simulated data only - not for operational use.**

## Build and run

```
cmake -B build -G Ninja
cmake --build build
.\build\afrm.exe
```

## Status (v0.3)

- `Weather` class: wind speed, wind direction and temperature, giving the
  headwind component for a heading
- `Route` class: distance and heading (single leg for now)
- Fleet moved into its own file (`src/Fleet.cpp`)
- Trip fuel now uses the route and weather instead of a single headwind value
- Contingency (5%) and final reserve (30 min) fuel
- Fuel on board limited to the aircraft's tank capacity

## Planned next
- Several tanks per aircraft, with balance checking
- Maximum range calculation
- Flight plan class with validation
