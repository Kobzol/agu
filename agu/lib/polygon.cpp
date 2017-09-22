#include "polygon.h"
#include "algo.h"

#include <random>
#include <iostream>

#define VERBOSE

Polygon Polygon::generate(float min, float max, int pointCount)
{
	std::default_random_engine engine;
	std::uniform_real_distribution<float> distribution(min, max);
	
	std::vector<Point> points;
	for (int i = 0; i < pointCount; i++)
	{
		points.push_back(Point(distribution(engine), distribution(engine)));
	}
	Polygon polygon;
	cv::convexHull(points, polygon.points);
	return polygon;
}

void Polygon::draw(cv::Mat& mat, bool fill)
{
	if (fill)
	{
		cv::fillConvexPoly(mat, this->points, cv::Scalar(1.0f, 0.0f, 0.0f));
	}
	else
	{
		int counter = 0;
		for (auto& line: this->getLines())
		{
			cv::putText(mat, std::to_string(counter++), line.first, CV_FONT_HERSHEY_SIMPLEX, 1.0f, cv::Scalar::all(1.0f));
			cv::line(mat, line.first, line.second, cv::Scalar(1.0f, 0.0f, 0.0f));
		}
	}
}

bool Polygon::test(float x, float y, cv::Mat& mat)
{ 
	return this->testConvex(this->points, x, y, mat);
	//return this->testConcave(x, y, mat);
}

bool Polygon::testConcave(float x, float y, cv::Mat& mat)
{
	Point guide = this->points[0] + (this->points[1] - this->points[0]) / 2.0f;
	Point src = Point(x, y);
	Point to = (guide - src);
	to /= std::sqrt(to.x * to.x + to.y * to.y);
	to *= 2000.0f;

	Line guidingLine = std::make_pair(src, src + to);

	/*cv::line(mat, guidingLine.first, guidingLine.second, cv::Scalar(0.0f, 0.0f, 1.0f));
	mat.at<cv::Vec3f>(guide.y, guide.x) = cv::Vec3f(0.0f, 1.0f, 0.0f);*/

	int count = 0;
	for (auto& line : this->getLines())
	{
		if (lineIntersects(guidingLine, line))
		{
			count++;
		}
	}

	return count % 2 != 0;
}

bool Polygon::testConvex(std::vector<Point> points, float x, float y, cv::Mat& mat)
{
	if (points.size() == 3)
	{
		std::vector<cv::Point> intPoints;
		for (auto& point : points)
		{
			intPoints.push_back(point);
		}
		cv::fillConvexPoly(mat, intPoints, cv::Scalar(0.0f, 1.0f, 1.0f));

		return liesInsideTriangle(Point(x, y), points);
	}

	size_t middleIndex = points.size() / 2;
	Point middle = points[middleIndex];
	Point target = Point(x, y);

	std::vector<Point> nextPoints;
	if (testPointRight(target, points[0], middle))
	{
#ifdef VERBOSE
		std::cerr << "going right" << std::endl;
#endif
		nextPoints = std::vector<Point>(points.begin() + middleIndex, points.end());
		nextPoints.emplace(nextPoints.begin(), points[0]);
	}
	else
	{
#ifdef VERBOSE
		std::cerr << "going left" << std::endl;
#endif
		nextPoints = std::vector<Point>(points.begin(), points.begin() + middleIndex + 1);
	}

	return testConvex(nextPoints, x, y, mat);
}

std::vector<Line> Polygon::getLines()
{
	std::vector<Line> lines;
	for (int i = 0; i < this->points.size(); i++)
	{
		int next = (i + 1) % this->points.size();
		lines.push_back(std::make_pair(this->points[i], this->points[next]));
	}

	return lines;
}
