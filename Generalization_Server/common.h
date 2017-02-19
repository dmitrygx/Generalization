#pragma once

#include <string>
#include <list>
#include <vector>
#include <utility>
#include <map>

typedef std::vector<double_t> curveCoord;
typedef std::vector<std::vector<double_t>> curvesCoord;
typedef std::pair<curveCoord, curveCoord> curve;
typedef std::pair<double_t, double_t> point;
typedef std::map<uint32_t, curve> curves;