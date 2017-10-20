#include "closeness.h"

static float distanceX(const Point& p1, const Point& p2)
{
    return std::abs(p1.x - p2.x);
}
static std::vector<Point> closestDivideX(std::vector<Point> points)
{
	if (points.size() < 2) return { Point(std::numeric_limits<float>::min(), 0), Point(std::numeric_limits<float>::max(), 0) };
    if (points.size() == 2) return points;

    std::vector<Point> halves[2];
	halves[0].insert(halves[0].end(), points.begin(), points.begin() + points.size() / 2);
	halves[1].insert(halves[1].end(), points.begin() + points.size() / 2, points.end());

	auto left = closestDivideX(halves[0]);
	auto right = closestDivideX(halves[1]);
	auto middle = std::vector<Point> { points[points.size() / 2 - 1], points[points.size() / 2] };

	auto leftDistance = distanceX(left[0], left[1]);
	auto rightDistance = distanceX(right[0], right[1]);
	auto middleDistance = distanceX(middle[0], middle[1]);

	if (leftDistance < rightDistance)
	{
		if (leftDistance < middleDistance) return left;
		else return middle;
	}
	else
	{
		if (rightDistance < middleDistance) return right;
		else return middle;
	}
}
std::vector<Point> closestpairX(std::vector<Point> points)
{
    std::sort(points.begin(), points.end(), [](const Point& p1, const Point& p2) {
        return p1.x < p2.x;
    });

    return closestDivideX(points);
}

static std::vector<Point> closestDivice2D(std::vector<Point> points)
{
	if (points.size() < 2) return { Point(std::numeric_limits<float>::min(), 0), Point(std::numeric_limits<float>::max(), 0) };
	if (points.size() == 2) return points;

	std::vector<Point> halves[2];
	halves[0].insert(halves[0].end(), points.begin(), points.begin() + points.size() / 2);
	halves[1].insert(halves[1].end(), points.begin() + points.size() / 2, points.end());

	auto left = closestDivice2D(halves[0]);
	auto right = closestDivice2D(halves[1]);
	auto middlePoint = (points[points.size() / 2 - 1] + points[points.size() / 2]) / 2;

	auto leftDistance = cv::norm(left[0] - left[1]);
	auto rightDistance = cv::norm(right[0] - right[1]);

	float minDistance = std::min(leftDistance, rightDistance);

	std::vector<Point> strip;
	for (auto& p : points)
	{
		if (std::abs(p.x - middlePoint.x) <= minDistance)
		{
			strip.push_back(p);
		}
	}

	std::sort(strip.begin(), strip.end(), [](const Point& p1, const Point& p2) {
		return p1.y < p2.y;
	});

	std::vector<Point> bestCross;
	float minCross = std::numeric_limits<float>::max();

	for (int i = 0; i < strip.size(); i++)
	{
		for (int j = i + 1; j < strip.size(); j++)
		{
			float dist = cv::norm(strip[i] - strip[j]);
			if (dist >= minDistance) break;
			if (dist < minCross)
			{
				minCross = dist;
				bestCross = { strip[i], strip[j] };
			}
		}
	}

	if (bestCross.size() > 0) return bestCross;
	else if (leftDistance < rightDistance) return left;
	else return right;
}
std::vector<Point> closestpair2D(std::vector<Point> points)
{
	std::sort(points.begin(), points.end(), [](const Point& p1, const Point& p2) {
		return p1.x < p2.x;
	});

    return closestDivice2D(points);
}
