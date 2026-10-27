cmake_minimum_required(VERSION 3.20)
project(AFRM VERSION 0.5.0 LANGUAGES CXX)

# Standard ISO C++17, no compiler-specific extensions
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(afrm
    src/main.cpp
    src/Aircraft.cpp
    src/FuelSystem.cpp
    src/FuelTank.cpp
    src/FlightPlan.cpp
    src/Route.cpp
    src/Weather.cpp
    src/FuelCalculator.cpp
    src/Fleet.cpp
)
target_include_directories(afrm PRIVATE include)

# Turn on compiler warnings
target_compile_options(afrm PRIVATE -Wall -Wextra -Wpedantic)
