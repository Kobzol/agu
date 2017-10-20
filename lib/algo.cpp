#include <functional>
#include <forward_list>
#include "algo.h"

cv::Vec2f toVec(const Point& point)
{
	return cv::Vec2f(point.x, point.y);
}

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
// https://matematika.cz/obecna-rovnice-primky
void getLineSlope(const Line& line, float& slopeA, float& slopeC)
{
	float a = line.first.x;
	float b = line.second.x;
	float c = line.first.y;
	float d = line.second.y;

	slopeA = (d - c) / (b - a);
	slopeC = -((d - c) / (b - a)) * a + c;
}

// https://en.wikipedia.org/wiki/Line-line_intersection
/*Point lineIntersection(const Line& line1, const Line& line2)
{
	float a, b, c, d;
	getLineSlope(line1, a, c);
	getLineSlope(line2, b, d);

	if (a == b) return INVALID_POINT;

	float x = (d - c) / (a - b);
	float y = a * x + c;

	float x1 = (x - line1.first.x) / (line1.second.x - line1.first.x);
	float y1 = (y - line1.first.y) / (line1.second.y - line1.first.y);
	float x2 = (x - line2.first.x) / (line2.second.x - line2.first.x);
	float y2 = (y - line2.first.y) / (line2.second.y - line2.first.y);

	if (x1 >= 0.0f && x1 <= 1.0f &&
		y1 >= 0.0f && y1 <= 1.0f &&
		x2 >= 0.0f && x2 <= 1.0f &&
		y2 >= 0.0f && y2 <= 1.0f)
	{
		return Point(x, y);
	}

	return INVALID_POINT;
}*/
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

// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
float linePointDistance(const Line& line, const Point& point)
{
	float x1 = line.first.x;
	float y1 = line.first.y;
	float x2 = line.second.x;
	float y2 = line.second.y;

	return std::abs((y2 - y1) * point.x - (x2 - x1) * point.y + x2 * y1 - y2 * x1) / std::sqrt(std::pow(y2 - y1, 2) + std::pow(x2 - x1, 2));
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

bool rectContains(const cv::Rect& rectangle, const cv::Point& point)
{
	return
		rectangle.x <= point.x &&
		point.x < (rectangle.x + rectangle.width) &&
		rectangle.y >= point.y &&
		point.y > (rectangle.y - rectangle.height);
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

void dumpPoints(std::ostream& os, const std::vector<Point>& points)
{
	for (auto& p : points)
	{
		os << "points.emplace_back(Point(" << p.x << ", OFFSET(" << p.y << ")) * SCALE);" << std::endl;
	}
}
