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

static void parametrizeLine(const Line& line, float &a, float &b, float &c)
{
	a = line.second.y - line.first.y;
	b = line.first.x - line.second.x;
	c = a * line.first.x + b * line.first.y;
}

// https://www.topcoder.com/community/data-science/data-science-tutorials/geometry-concepts-line-intersection-and-its-applications/
Point lineIntersection(const Line& line1, const Line& line2)
{
	float a1, b1, c1;
	float a2, b2, c2;

	parametrizeLine(line1, a1, b1, c1);
	parametrizeLine(line2, a2, b2, c2);

	float det = a1 * b2 - a2 * b1;
	if (det == 0) return INVALID_POINT;

	Point cross((b2*c1 - b1*c2) / det, (a1*c2 - a2*c1) / det);
	Line lines[2] = { line1, line2 };

	for (int i = 0; i < 2; i++)
	{
		if (!(cross.x >= std::min(lines[i].first.x, lines[i].second.x) &&
			cross.x <= std::max(lines[i].first.x, lines[i].second.x) &&
			cross.y >= std::min(lines[i].first.y, lines[i].second.y) &&
			cross.y <= std::max(lines[i].first.y, lines[i].second.y)))
		{
			return INVALID_POINT;
		}
	}

	return cross;
}

// http://www.geeksforgeeks.org/orientation-3-ordered-points/
int testPointCCW(const Point& p, const Point& p1, const Point& p2)
{
	//return (p.y - p1.y) * (p2.x - p1.x) - (p.x - p1.x) * (p2.y - p1.y) < 0;
	float value = (p.y - p1.y) * (p2.x - p.x) - (p2.y - p.y) * (p.x - p1.x);

	if (value < 0.0f) return -1;
	else if (value > 0.0f) return 1;
	return 0;
}

bool testPointRight(const Point& p, const Point& p1, const Point& p2)
{
	return testPointCCW(p, p1, p2) > 0;
}

bool liesInsideTriangle(const Point& point, const std::vector<Point>& triangle)
{
	int directions[3];
	for (int i = 0; i < 3; i++)
	{
		int next = (i + 1) % 3;
		directions[i] = testPointCCW(point, triangle[i], triangle[next]);
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
