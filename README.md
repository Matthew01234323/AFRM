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

## Status (v0.4)

- `FuelSystem` class: each aircraft now has LEFT and RIGHT tanks
- Fuel entered per tank, limited to each tank's capacity
- Tank balance check, with automatic fuel transfer if unbalanced
- Low-tank warnings
- Maximum range (full tanks, keeping reserves)
- Weather and route as in v0.3

## Planned next
- Engine efficiency, max weight and mid-flight refuelling
- Flight plan class with validation
