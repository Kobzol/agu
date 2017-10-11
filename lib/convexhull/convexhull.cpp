#include "convexhull.h"

#include "../algo.h"

static int findExtremePointX(const std::vector<Point>& points)
{
    int extreme = 0;
    size_t size = points.size();
    for (int i = 1; i < size; i++)
    {
        if (points[i].x < points[extreme].x)
        {
            extreme = i;
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
	Point p = points[extreme];

	std::swap(points[extreme], points[points.size() - 1]);
	points.resize(points.size() - 1);

	std::sort(points.begin(), points.end(), [&p, &axis](const Point& p1, const Point& p2) {
		return cv::normalize(toVec(p - p1)).dot(axis) < cv::normalize(toVec(p - p2)).dot(axis);
		//return cv::fastAtan2((p - p1).y, (p - p1).x) < cv::fastAtan2((p - p2).y, (p - p2).x);
	});
	points.insert(points.begin(), p);
	
	std::vector<Point> hull = { points[0], points[1] };

	for (int i = 0; i < points.size(); i++)
	{
		while (testPointRight(hull[hull.size() - 1], hull[hull.size() - 2], points[i]))
		{
			if (hull.size() > 2)
			{
				hull.pop_back();
				continue;
			}
			else i++;
		}

		hull.push_back(points[i]);
	}

	return hull;
}
