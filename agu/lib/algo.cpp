#include "algo.h"
#include <iostream>

// http://www.cdn.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
bool lineIntersects(const Line& line1, const Line& line2)
{
	auto a = line1.first;
	auto b = line1.second;
	auto c = line2.first;
	auto d = line2.second;

	return	testPointRight(a, c, d) != testPointRight(b, c, d) &&
			testPointRight(c, a, b) != testPointRight(d, a, b);
}

// http://www.geeksforgeeks.org/orientation-3-ordered-points/
int testPointccw(const Point& p, const Point& p1, const Point& p2)
{
	//return (p.y - p1.y) * (p2.x - p1.x) - (p.x - p1.x) * (p2.y - p1.y) < 0;
	float value = (p.y - p1.y) * (p2.x - p.x) - (p2.y - p.y) * (p.x - p1.x);

	if (value < 0.0f) return -1;
	else if (value > 0.0f) return 1;
	return 0;
}

bool testPointRight(const Point& p, const Point& p1, const Point& p2)
{
	return testPointccw(p, p1, p2) < 0;
}

bool liesInsideTriangle(const Point& point, const std::vector<Point>& triangle)
{
	int directions[3];
	for (int i = 0; i < 3; i++)
	{
		int next = (i + 1) % 3;
		directions[i] = testPointccw(point, triangle[i], triangle[next]);
	}

	if (directions[0] == 0 || directions[1] == 0 || directions[2] == 0)
	{
		std::cerr << "EDGE CASE!!!" << std::endl;
	}

	return directions[0] == directions[1] && directions[1] == directions[2];
}

Point weightCenter(const std::vector<Point>& points)
{
	Point weight;

	for (auto& point : points)
	{
		weight += point;
	}

	return weight / (float) points.size();
}
