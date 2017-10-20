#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include "common.h"

class Polygon
{
public:
	static Polygon generate(float min, float max, int pointCount);

	void draw(cv::Mat& mat, bool fill=false);
	bool test(float x, float y, cv::Mat& mat);

	std::vector<Point> points;

private:
	bool testConcave(float x, float y, cv::Mat& mat);
	bool testConvex(std::vector<Point> points, float x, float y, cv::Mat& mat);

	std::vector<Line> getLines();
};
