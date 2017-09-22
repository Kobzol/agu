#pragma once

#include "polygon.h"

bool lineIntersects(const Line& line1, const Line& line2);
int testPointccw(const Point& p, const Point& p1, const Point& p2);
bool testPointRight(const Point& point, const Point& from, const Point& to);
bool liesInsideTriangle(const Point& point, const std::vector<Point>& triangle);

Point weightCenter(const std::vector<Point>& points);
