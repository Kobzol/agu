#include "convexhull.h"
#include "../algo.h"

#include <forward_list>

template <bool smallest = true>
static int findExtremePointX(const std::vector<Point>& points)
{
    int extreme = 0;
    size_t size = points.size();
    for (int i = 1; i < size; i++)
    {
		if (smallest)
		{
			if (points[i].x < points[extreme].x)
			{
				extreme = i;
			}
		}
		else
		{
			if (points[i].x > points[extreme].x)
			{
				extreme = i;
			}
		}
    }

    return extreme;
}
static int findExtremePointY(const std::vector<Point>& points)
{
	int extreme = 0;
	size_t size = points.size();
	for (int i = 1; i < size; i++)
	{
		if (points[i].y < points[extreme].y)
		{
			extreme = i;
		}
	}

	return extreme;
}

std::vector<Point> giftWrap(std::vector<Point> points)
{
    int extreme = findExtremePointX(points);
    std::vector<Point> hull = { points[extreme] };
	int lastHull = extreme;

    while (true)
    {
		int nextHull = 0;
		for (int i = 1; i < points.size(); i++)
		{
			if (lastHull == nextHull || testPointRight(points[i], points[lastHull], points[nextHull]))
			{
				nextHull = i;
			}
		}
        
		hull.emplace_back(points[nextHull]);
		if (points[nextHull] == hull[0]) break;
		lastHull = nextHull;
    }

    return hull;
}

std::vector<Point> grahamScan(std::vector<Point> points)
{
	Point axis(1, 0);
	int extreme = findExtremePointY(points);
	Point point = points[extreme];

	std::swap(points[extreme], points[0]);

	std::sort(points.begin() + 1, points.end(), [&point, &axis](const Point& p1, const Point& p2) {
		return cv::normalize(toVec(point - p1)).dot(axis) < cv::normalize(toVec(point - p2)).dot(axis);
		//return cv::fastAtan2((point - p1).y, (point - p1).x) < cv::fastAtan2((point - p2).y, (point - p2).x);
	});
	
	std::vector<Point> hull = { points[0], points[1], points[2] };

	for (int i = 3; i < points.size(); i++)
	{
		while (testPointRight(hull[hull.size() - 1], hull[hull.size() - 2], points[i]))
		{
			if (hull.size() > 2)
			{
				hull.pop_back();
			}
		}
		hull.push_back(points[i]);
	}

	return hull;
}

static void findHull(std::forward_list<Point>& hull, std::vector<Point>& points,
	std::forward_list<Point>::iterator start, std::forward_list<Point>::iterator end)
{
	if (points.size() < 1) return;

	Line line(*start, *end);
	int maxIndex = 0;
	for (int i = 1; i < points.size(); i++)
	{
		if (points[i] == *start || points[i] == *end) continue;
		if (linePointDistance(line, points[i]) > linePointDistance(line, points[maxIndex]))
		{
			maxIndex = i;
		}
	}

	Point c = points[maxIndex];
	auto cIterator = hull.insert_after(start, c);

	std::swap(points[maxIndex], points[points.size() - 1]);

	std::vector<Point> subpoints[2];
	for (int i = 0; i < points.size() - 1; i++)
	{
		Point& p = points[i];
		if (testPointRight(p, *start, c))
		{
			subpoints[0].push_back(p);
		}
		else if (testPointRight(p, c, *end))
		{
			subpoints[1].push_back(p);
		}
	}

	findHull(hull, subpoints[0], start, cIterator);
	findHull(hull, subpoints[1], cIterator, end);
}
std::vector<Point> quickHull(std::vector<Point> points)
{
	Point left = points[findExtremePointX<true>(points)];
	Point right = points[findExtremePointX<false>(points)];

	std::forward_list<Point> hull = { left, right };
	std::vector<Point> subpoints[2];
	
	for (auto& p : points)
	{
		if (p == left || p == right) continue;
		subpoints[testPointRight(p, left, right)].push_back(p);
	}

	auto start = hull.begin();
	auto end = hull.begin();
	end++;

	findHull(hull, subpoints[1], start, end);
	findHull(hull, subpoints[0], end, start);

	std::vector<Point> hullVector(hull.begin(), hull.end());
	return hullVector;
}
