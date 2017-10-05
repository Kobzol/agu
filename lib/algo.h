#pragma once

#include "polygon.h"

#define INVALID_POINT (Point(-1, -1))

bool lineIntersects(const Line& line1, const Line& line2);
Point lineIntersection(const Line& line1, const Line& line2);

int testPointCCW(const Point& p, const Point& p1, const Point& p2);
bool testPointRight(const Point& point, const Point& from, const Point& to);
bool liesInsideTriangle(const Point& point, const std::vector<Point>& triangle);

Point weightCenter(const std::vector<Point>& points);
