#include "convexhull.h"
#include "../algo.h"

#include <forward_list>
#include <functional>

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
template <bool smallest = true>
static int findExtremePointY(const std::vector<Point>& points)
{
	int extreme = 0;
	size_t size = points.size();
	for (int i = 1; i < size; i++)
	{
		if (smallest)
		{
			if (points[i].y < points[extreme].y)
			{
				extreme = i;
			}
		}
		else
		{
			if (points[i].y > points[extreme].y)
			{
				extreme = i;
			}
		}
	}

	return extreme;
}

std::vector<Point> giftWrap(std::vector<Point> points)
{
    int extreme = findExtremePointX<true>(points);
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
        
		if (points[nextHull] == hull[0]) break;
		hull.push_back(points[nextHull]);
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

template <typename Comparator>
static std::pair<int, int> twoFinger(const std::vector<Point>& left, const std::vector<Point>& right,
	int l, int r, int leftOffset, Line& axis, float bestY, Comparator comparator)
{
	std::vector<Point> joined(left.begin(), left.end());
	joined.insert(joined.end(), right.begin(), right.end());

	int rightOffset = leftOffset;
	while (true)
	{
		int nextl = ((l + leftOffset) + left.size()) % left.size();
		float ly = lineIntersection(Line(left[nextl], right[r]), axis).y;
		if (comparator(ly, bestY))
		{
			l = nextl;
			bestY = ly;
		}

		int nextr = ((r + rightOffset) + right.size()) % right.size();
		float ry = lineIntersection(Line(left[l], right[nextr]), axis).y;
		if (comparator(ry, bestY))
		{
			r = nextr;
			bestY = ry;
		}

		if (l != nextl && r != nextr) break;
	}

	return { l, r };
}
static std::vector<Point> merge(std::vector<Point> left, std::vector<Point> right)
{
	int leftPivot = findExtremePointX<false>(left);
	int rightPivot = findExtremePointX<true>(right);
	Point axisDown((left[leftPivot].x + right[rightPivot].x) / 2, 0);
	Point axisUp(axisDown.x, 1000);
	Line axis(axisDown, axisUp);

	auto highestPair = twoFinger(left, right, leftPivot, rightPivot, 1, axis, axisDown.y, std::greater<float>());
	auto lowestPair = twoFinger(left, right, leftPivot, rightPivot, -1, axis, axisUp.y, std::less<float>());

	std::vector<Point> hull;
	for (int i = lowestPair.first; i != highestPair.first; i = (i + 1) % left.size())
	{
		hull.push_back(left[i]);
	}
	hull.push_back(left[highestPair.first]);
	for (int i = highestPair.second; i != lowestPair.second; i = (i + 1) % right.size())
	{
		hull.push_back(right[i]);
	}
	hull.push_back(right[lowestPair.second]);

	return hull;
}
static std::vector<Point> divide(std::vector<Point> points)
{
	if (points.size() <= 5) return giftWrap(points);

	std::vector<Point> halves[2];
	for (int i = 0; i < points.size() / 2; i++)
	{
		halves[0].push_back(points[i]);
	}
	for (int i = points.size() / 2; i < points.size(); i++)
	{
		halves[1].push_back(points[i]);
	}

	return merge(divide(halves[0]), divide(halves[1]));
}
std::vector<Point> divideAndConquer(std::vector<Point> points)
{
	std::sort(points.begin(), points.end(), [](const Point& p1, const Point& p2) {
		return p1.x < p2.x;
	});

	return divide(points);
}

void drawHull(cv::Mat& image, const std::vector<Point>& points, const std::vector<Point>& convexHull)
{
	for (int i = 0; i < points.size(); i++)
	{
		auto p = Point(points[i].x, image.rows - points[i].y);
		cv::circle(image, p, 2, cv::Scalar(1.0f, 1.0f, 1.0f));
		cv::putText(image, std::to_string(i), p + Point(0.2f, 0.0f), CV_FONT_HERSHEY_COMPLEX_SMALL, 0.5f, cv::Scalar(0.0f, 1.0f, 0.0f), 1, CV_AA);
	}

	for (int i = 0; i < convexHull.size(); i++)
	{
		auto from = convexHull[i];
		auto to = convexHull[(i + 1) % convexHull.size()];
		from.y = image.rows - from.y;
		to.y = image.rows - to.y;

		cv::line(image, from, to, cv::Scalar(0.0f, 0.0f, 1.0f));
	}
}
