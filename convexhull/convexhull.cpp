#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <algo.h>

#include <convexhull/convexhull.h>

static std::vector<Point> points;
static bool hullCalculated = false;

#define CONVEX_HULL_METHOD (divideAndConquer)
#define OFFSET(x) (x)
#define DRAW_OFFSET(x) (600 - x)
#define SCALE (1.0f)

static void callback(int event, int x, int y, int flags, void* userdata)
{
    if (event != cv::EVENT_LBUTTONDOWN && event != cv::EVENT_RBUTTONDOWN) return;

    cv::Mat mat = cv::Mat(600, 600, CV_32FC3);

	if (event == cv::EVENT_LBUTTONDOWN)
    {
        if (hullCalculated)
        {
            hullCalculated = false;
            points.clear();
        }

        points.emplace_back(static_cast<float>(x), static_cast<float>(DRAW_OFFSET(y)));
		drawHull(mat, points, {});

		/*std::ofstream fs("points.txt");
		fs << std::endl;
		for (auto& p : points)
		{
			fs << "points.emplace_back(Point(" << p.x << ", OFFSET(" << p.y << ")) * SCALE);" << std::endl;
		}*/
	}
	else
    {
        hullCalculated = true;
		drawHull(mat, points, CONVEX_HULL_METHOD(points));
	}

    cv::imshow("Convexhull", mat);
    cv::waitKey(0);
}

void convexhull()
{
	points.emplace_back(Point(141, OFFSET(477)) * SCALE);
	points.emplace_back(Point(56, OFFSET(455)) * SCALE);
	points.emplace_back(Point(111, OFFSET(378)) * SCALE);
	points.emplace_back(Point(173, OFFSET(424)) * SCALE);
	points.emplace_back(Point(281, OFFSET(560)) * SCALE);
	points.emplace_back(Point(347, OFFSET(576)) * SCALE);
	points.emplace_back(Point(354, OFFSET(518)) * SCALE);
	points.emplace_back(Point(297, OFFSET(484)) * SCALE);

	cv::Mat mat = cv::Mat(600, 600, CV_32FC3);
    drawHull(mat, points, {});

	cv::namedWindow("Convexhull", 1);
	cv::setMouseCallback("Convexhull", callback, nullptr);
	cv::imshow("Convexhull", mat);
	cv::waitKey(0);
}
