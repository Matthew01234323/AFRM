# AFRM test graphs

Made from `test_results_2026-10-28_16-03-57.csv` and `data\test_flights.csv`.

## Expected results

```mermaid
%%{init: {"themeVariables": {"pie1": "#0ca30c", "pie2": "#d03b3b", "pie3": "#fab219"}}}%%
pie showData
    title Expected results of the 3000 test flights
    "VALID" : 1680
    "NOT VALID" : 720
    "REJECTED" : 600
```

Split of expected results across all test flights.

## Tests passed per category

```mermaid
%%{init: {"themeVariables": {"xyChart": {"plotColorPalette": "#2a78d6"}}}}%%
xychart-beta
    title "Tests passed per category (3000 / 3000 passed)"
    x-axis ["typical", "boundary", "invalid", "wind", "fuel_status", "tanks"]
    y-axis "Tests passed" 0 --> 1300
    bar [1200, 600, 400, 300, 300, 200]
```

Number of tests passed in each category.

## Rejected inputs by cause

```mermaid
%%{init: {"themeVariables": {"xyChart": {"plotColorPalette": "#2a78d6"}}}}%%
xychart-beta horizontal
    title "Rejected inputs by cause (600 tests)"
    x-axis ["Tank fuel", "Distance", "Temperature", "Aircraft", "Wind speed", "Heading", "Wrong tank count", "Wind direction", "Aircraft (decimal)"]
    y-axis "Tests rejected" 0 --> 100
    bar [95, 78, 72, 71, 70, 68, 66, 57, 23]
```

Which input caused each rejected test.

## Required fuel against distance

```mermaid
%%{init: {"themeVariables": {"xyChart": {"plotColorPalette": "#2a78d6, #eb6834, #1baf7a, #eda100"}}}}%%
xychart-beta
    title "Average required fuel by distance (typical tests)"
    x-axis "Distance (nm)" ["0-499", "500-999", "1000-1499", "1500-1999"]
    y-axis "Required fuel (tonnes)" 0 --> 19
    line [2.9, 5.5, 8.6, 11.4]
    line [1.0, 2.3, 3.6, 4.7]
    line [4.1, 7.6, 12.6, 16.7]
    line [1.0, 2.0, 3.1, 4.1]
```

Average required fuel in each 500 nm band (typical tests). Lines: blue = SIM-001 Narrow-body jet, orange = SIM-002 Regional turboprop, green = SIM-003 Military transport, yellow = SIM-004 Business jet.

## Effect of wind on required fuel

```mermaid
%%{init: {"themeVariables": {"xyChart": {"plotColorPalette": "#2a78d6"}}}}%%
xychart-beta
    title "Effect of wind on required fuel (SIM-001)"
    x-axis "Headwind (kt, negative = tailwind)" [-173, -130, -87, -43, -22, 0, 22, 43, 87, 130, 173]
    y-axis "Required fuel (kg)" 2800 --> 5200
    line [2935, 3061, 3207, 3379, 3476, 3583, 3701, 3832, 4139, 4530, 5043]
```

Required fuel for SIM-001 in the wind tests, against the headwind component.

