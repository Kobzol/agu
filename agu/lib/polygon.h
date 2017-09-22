#pragma once

#include <opencv2/world.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

using Point = cv::Point2f;
using Line = std::pair<cv::Point2f, cv::Point2f>;

class Polygon
{
public:
	static Polygon generate(float min, float max, int pointCount);

	void draw(cv::Mat& mat, bool fill=false);
	bool test(float x, float y, cv::Mat& mat);

	std::vector<Point> points;

private:
	bool testConcave(float x, float y, cv::Mat& mat);
	bool testConvex(size_t startIndex, size_t endIndex, Point lastWeight, float x, float y, cv::Mat& mat);

	std::vector<Line> getLines();
};