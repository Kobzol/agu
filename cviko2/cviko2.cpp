#include <iostream>

#include <opencv2/opencv.hpp>
#include <algo.h>

#include <planesweep/planesweep.h>
#include <draw.h>

static std::vector<Line> lines;
static int pointIndex = 0;
static Point endpoints[2];

std::vector<Point> intersectionsNaive(const std::vector<Line>& lines)
{
	std::vector<Point> intersections;

	for (int i = 0; i < lines.size(); i++)
	{
		for (int j = i + 1; j < lines.size(); j++)
		{
			Point cross = lineIntersection(lines[i], lines[j]);
			if (cross != INVALID_POINT)
			{
				intersections.push_back(cross);
			}
		}
	}

	return intersections;
}


static void drawAll(cv::Mat& image, const std::vector<Line>& lines)
{
	drawLines(image, lines, cv::Scalar(1.0f, 1.0f, 1.0f));

	std::vector<Point> intersectsNaive = intersectionsNaive(lines);
	drawPoints(image, intersectsNaive, cv::Scalar(0.0f, 1.0f, 0.0f), 6);

	std::vector<Point> intersectsBentley = bentleyOttmann(lines);
	drawPoints(image, intersectsBentley, cv::Scalar(0.0f, 0.0f, 1.0f), 4);
}

static void callback(int event, int x, int y, int flags, void* userdata)
{
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		endpoints[pointIndex] = Point(static_cast<float>(x), static_cast<float>(y));
		pointIndex = (pointIndex + 1) % 2;

		if (pointIndex == 0)
		{
			lines.emplace_back(endpoints[0], endpoints[1]);
		}

		cv::Mat mat = cv::Mat(600, 600, CV_32FC3);
		drawLines(mat, lines, cv::Scalar(1.0f, 1.0f, 1.0f));
		cv::imshow("Bentley-Ottmann", mat);
		cv::waitKey(0);
	}
	else if (event == cv::EVENT_RBUTTONDOWN)
	{
		cv::Mat mat = cv::Mat(600, 600, CV_32FC3);
		drawAll(mat, lines);
		cv::imshow("Bentley-Ottmann", mat);
		cv::waitKey(0);
	}
	else if (event == cv::EVENT_MOUSEMOVE)
	{
		std::cerr << x << " " << y << std::endl;
	}
}

#define OFFSET(x) (x)

void cviko2()
{
	lines.emplace_back(Point(6, OFFSET(63)), Point(418, OFFSET(335)));
	lines.emplace_back(Point(49, OFFSET(176)), Point(391, OFFSET(161)));
	lines.emplace_back(Point(123, OFFSET(15)), Point(541, OFFSET(358)));
	lines.emplace_back(Point(140, OFFSET(335)), Point(564, OFFSET(403)));
	lines.emplace_back(Point(326, OFFSET(459)), Point(381, OFFSET(153)));
/*	lines.emplace_back(Point(86, OFFSET(209)), Point(243, OFFSET(136)));
	lines.emplace_back(Point(118, OFFSET(102)), Point(263, OFFSET(232)));
	lines.emplace_back(Point(362, OFFSET(141)), Point(193, OFFSET(236)));
	lines.emplace_back(Point(196, OFFSET(116)), Point(310, OFFSET(101)));
	lines.emplace_back(Point(327, OFFSET(212)), Point(429, OFFSET(170)));*/

	cv::Mat mat = cv::Mat(600, 600, CV_32FC3);
	drawAll(mat, lines);

	cv::namedWindow("Bentley-Ottmann", 1);
	cv::setMouseCallback("Bentley-Ottmann", callback, nullptr);
	cv::imshow("Bentley-Ottmann", mat);
	cv::waitKey(0);
}
