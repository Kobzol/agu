#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <algo.h>

#include <draw.h>
#include <kdtree/kdtree.h>

static std::vector<Point> points;
static std::unique_ptr<KDTree> tree;
static int pointIndex = 0;
static Point rectPoints[2];

#define OFFSET(x) (x)
#define SCALE (3.0f)

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
			cv::Rect rect(rectPoints[0].x, rectPoints[0].y, std::abs(rectPoints[1].x - rectPoints[0].x), std::abs(rectPoints[1].y - rectPoints[0].y));
			auto found = tree->find(points, rect);

			cv::Rect drawRect(rect.x, image.rows - rect.y, rect.width, rect.height);
			drawHull(image, points);
			cv::rectangle(image, drawRect, cv::Scalar(0.0f, 1.0f, 0.0f), 1, CV_AA);
			drawPoints(image, found, cv::Scalar(0.0f, 0.0f, 1.0f), 4.0f);

			cv::imshow("KD-tree", image);
			cv::waitKey(0);
		}
	}
}

void kdtree()
{
	points.emplace_back(Point(10, OFFSET(80)) * SCALE);
	points.emplace_back(Point(20, OFFSET(30)) * SCALE);
	points.emplace_back(Point(25, OFFSET(60)) * SCALE);
	points.emplace_back(Point(30, OFFSET(65)) * SCALE);
	points.emplace_back(Point(35, OFFSET(20)) * SCALE);
	points.emplace_back(Point(40, OFFSET(120)) * SCALE);
	points.emplace_back(Point(45, OFFSET(25)) * SCALE);
	points.emplace_back(Point(50, OFFSET(35)) * SCALE);
	points.emplace_back(Point(70, OFFSET(110)) * SCALE);
	points.emplace_back(Point(80, OFFSET(50)) * SCALE);
	points.emplace_back(Point(95, OFFSET(105)) * SCALE);

	cv::Mat mat = cv::Mat(600, 600, CV_32FC3);
    drawHull(mat, points);

	cv::namedWindow("KD-tree", 1);
	cv::setMouseCallback("KD-tree", callback, nullptr);
	cv::imshow("KD-tree", mat);

    tree = KDTree::buildTree(points);
	std::cerr << tree->getPointCount() << std::endl;

	cv::waitKey(0);
}
