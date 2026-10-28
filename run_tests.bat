@echo off
REM Builds AFRM and runs the automated tests against the sample flights.
REM Results are saved in the results folder.

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

build\afrm_tests.exe data\test_flights.csv results
pause
