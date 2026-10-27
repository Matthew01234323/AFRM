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

## Status (v0.2)

- Aircraft and fuel tank are now classes
- Choice of two simulated aircraft
- Trip fuel includes a headwind / tailwind
- Contingency (5%) and final reserve (30 min) fuel
- Fuel on board limited to the aircraft's tank capacity

## Planned next
- Several tanks per aircraft, with balance checking
- Weather (wind speed and direction) and routes
- Flight plan class with validation
