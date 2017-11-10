#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>

#include <algo.h>
#include <closeness/closeness.h>
#include <draw.h>

static std::vector<Point> points;
static bool closenessCalculated = false;

#define CLOSENESS_METHOD (closestpair2D)
#define OFFSET(x) (x)
#define SCALE (1.0f)

static void callback(int event, int x, int y, int flags, void* userdata)
{
    if (event != cv::EVENT_LBUTTONDOWN && event != cv::EVENT_RBUTTONDOWN) return;

    cv::Mat mat = cv::Mat(600, 600, CV_32FC3);

	if (event == cv::EVENT_LBUTTONDOWN)
    {
        if (closenessCalculated)
        {
			closenessCalculated = false;
            points.clear();
        }

        points.emplace_back(static_cast<float>(x), static_cast<float>(mat.rows - y));
	}
	else
    {
		closenessCalculated = true;
		auto closest = CLOSENESS_METHOD(points);
		for (auto& p : closest)
		{
			cv::circle(mat, Point(p.x, mat.rows - p.y), 5, cv::Scalar(0.0f, 1.0f, 0.0f), 1, CV_AA);
		}

		std::ofstream fs("points.txt");
		dumpPoints(fs, points);
	}

	drawHull(mat, points);
    cv::imshow("Closeness", mat);
    cv::waitKey();
}

void closeness()
{
	points.emplace_back(Point(135, OFFSET(517)) * SCALE);
	points.emplace_back(Point(173, OFFSET(516)) * SCALE);
	points.emplace_back(Point(232, OFFSET(517)) * SCALE);
	points.emplace_back(Point(248, OFFSET(517)) * SCALE);
	points.emplace_back(Point(321, OFFSET(518)) * SCALE);

	cv::Mat mat = cv::Mat(600, 600, CV_32FC3);
    drawHull(mat, points);

	cv::namedWindow("Closeness", 1);
	cv::setMouseCallback("Closeness", callback, nullptr);
	cv::imshow("Closeness", mat);
	cv::waitKey();
}
