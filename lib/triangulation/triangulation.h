#pragma once

#include <vector>

#include "../common.h"
#include "../planarmap/planarmap.h"

std::vector<Line> mapToVoronoi(Planarmap& map);
std::vector<Line> mapToLines(Planarmap& map);
Planarmap triangulate(std::vector<Point> points);
