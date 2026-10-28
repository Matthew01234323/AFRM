@echo off
REM Makes Mermaid graphs from the test data and the latest test results.
REM Run run_tests.bat first. Graphs are saved in results\graphs.

cd /d "%~dp0"

if not exist build (
    cmake -B build -G Ninja
)
cmake --build build
if errorlevel 1 (
    echo Build failed.
    pause
    exit /b 1
)

build\afrm_graphs.exe data\test_flights.csv results
pause
