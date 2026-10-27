#pragma once
#include <vector>

#include "Aircraft.hpp"

// All aircraft the AFRM knows about. Add new aircraft in Fleet.cpp.
std::vector<Aircraft> loadFleet();
