#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <algo.h>

#include <draw.h>
#include <tree/kdtree.h>
#include <tree/intervaltree.h>

#define OFFSET(x) (x)
#define SCALE (1.0f)
using Tree = IntervalTree;

static std::vector<Point> points;
static std::unique_ptr<Tree> searchtree;
static int pointIndex = 0;
static Point rectPoints[2];

static void callback(int event, int x, int y, int flags, void* userdata)
{
    if (event != cv::EVENT_LBUTTONDOWN && event != cv::EVENT_RBUTTONDOWN) return;

	cv::Mat image = cv::Mat(600, 600, CV_32FC3);
	Point point(static_cast<float>(x), static_cast<float>(image.rows - y));

	if (event == cv::EVENT_LBUTTONDOWN)
	{
		rectPoints[pointIndex] = point;
		pointIndex = (pointIndex + 1) % 2;
		if (pointIndex == 0)
		{
			if (rectPoints[1].x < rectPoints[0].x)
			{
				std::swap(rectPoints[0], rectPoints[1]);
			}

			cv::Rect rect(rectPoints[0].x, rectPoints[0].y, std::abs(rectPoints[1].x - rectPoints[0].x), std::abs(rectPoints[1].y - rectPoints[0].y));
			auto found = searchtree->find(points, rect);

			cv::Rect drawRect(rect.x, image.rows - rect.y, rect.width, rect.height);
			drawHull(image, points);
			cv::rectangle(image, drawRect, cv::Scalar(0.0f, 1.0f, 0.0f), 1, CV_AA);
			drawPoints(image, found, cv::Scalar(0.0f, 0.0f, 1.0f), 4.0f);

			cv::imshow("Tree", image);
			cv::waitKey(0);
		}
	}
}

void tree()
{
	/*points.emplace_back(Point(10, OFFSET(80)) * SCALE);
	points.emplace_back(Point(20, OFFSET(30)) * SCALE);
	points.emplace_back(Point(25, OFFSET(60)) * SCALE);
	points.emplace_back(Point(30, OFFSET(65)) * SCALE);
	points.emplace_back(Point(35, OFFSET(20)) * SCALE);
	points.emplace_back(Point(40, OFFSET(120)) * SCALE);
	points.emplace_back(Point(45, OFFSET(25)) * SCALE);
	points.emplace_back(Point(50, OFFSET(35)) * SCALE);
	points.emplace_back(Point(70, OFFSET(110)) * SCALE);
	points.emplace_back(Point(80, OFFSET(50)) * SCALE);
	points.emplace_back(Point(95, OFFSET(105)) * SCALE);*/
	points.emplace_back(Point(31, OFFSET(354)) * SCALE);
	points.emplace_back(Point(71, OFFSET(238)) * SCALE);
	points.emplace_back(Point(113, OFFSET(334)) * SCALE);
	points.emplace_back(Point(156, OFFSET(374)) * SCALE);
	points.emplace_back(Point(189, OFFSET(196)) * SCALE);
	points.emplace_back(Point(235, OFFSET(454)) * SCALE);
	points.emplace_back(Point(270, OFFSET(224)) * SCALE);
	points.emplace_back(Point(297, OFFSET(267)) * SCALE);
	points.emplace_back(Point(358, OFFSET(422)) * SCALE);
	points.emplace_back(Point(398, OFFSET(254)) * SCALE);
	points.emplace_back(Point(427, OFFSET(421)) * SCALE);

	cv::Mat mat = cv::Mat(600, 600, CV_32FC3);
    drawHull(mat, points);

	cv::namedWindow("Tree", 1);
	cv::setMouseCallback("Tree", callback, nullptr);
	cv::imshow("Tree", mat);

    searchtree = Tree::buildTree(points);

	cv::waitKey(0);
}
