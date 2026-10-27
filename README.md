# AFRM - Aircraft Fuel and Range Management (prototype)

Command-line prototype that calculates the fuel needed for a flight and
checks whether the fuel on board is sufficient.

**Simulated data only - not for operational use.**

## Build and run

```
cmake -B build -G Ninja
cmake --build build
./build/afrm
```

## Status

Early prototype. Currently calculates taxi, trip (cruise only), contingency
(5%) and final reserve (30 min) fuel for one hard-coded aircraft.

Planned next:
- Split trip fuel into take-off, climb, cruise, descent and landing
- Alternate fuel
- Load aircraft and flights from data files
- Unit tests (GoogleTest)
