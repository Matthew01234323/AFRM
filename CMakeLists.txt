cmake_minimum_required(VERSION 3.20)
project(AFRM VERSION 0.6.0 LANGUAGES CXX)

# Standard ISO C++17, no compiler-specific extensions
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# All the classes go in a library so the main program and the tests share them
add_library(afrm_core STATIC
    src/Aircraft.cpp
    src/FuelSystem.cpp
    src/FuelTank.cpp
    src/FlightPlan.cpp
    src/Route.cpp
    src/Weather.cpp
    src/FuelCalculator.cpp
    src/Fleet.cpp
)
target_include_directories(afrm_core PUBLIC include)
target_compile_options(afrm_core PUBLIC -Wall -Wextra -Wpedantic)

# Main program
add_executable(afrm src/main.cpp)
target_link_libraries(afrm PRIVATE afrm_core)

# Automated tests (run with run_tests.bat)
add_executable(afrm_tests tests/test_runner.cpp)
target_link_libraries(afrm_tests PRIVATE afrm_core)

# Graph maker (run with make_graphs.bat, after the tests)
add_executable(afrm_graphs tests/make_graphs.cpp)
target_link_libraries(afrm_graphs PRIVATE afrm_core)
