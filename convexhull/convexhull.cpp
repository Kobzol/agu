#include <iostream>

#include <opencv2/opencv.hpp>
#include <algo.h>

#include <convexhull/convexhull.h>

static std::vector<Point> points;
static bool hullCalculated = false;

#define CONVEX_HULL_METHOD (quickHull)

static void drawPoints(cv::Mat& image, const std::vector<Point>& points, const std::vector<Point>& convexHull)
{
    for (int i = 0; i < points.size(); i++)
    {
		auto& p = points[i];
        cv::circle(image, p, 2, cv::Scalar(1.0f, 1.0f, 1.0f));
		cv::putText(image, std::to_string(i), p + Point(0.2f, 0.0f), CV_FONT_HERSHEY_COMPLEX_SMALL, 0.5f, cv::Scalar(0.0f, 1.0f, 0.0f), 1, CV_AA);
    }

	for (int i = 0; i < convexHull.size(); i++)
	{
		cv::line(image, convexHull[i], convexHull[(i + 1) % convexHull.size()], cv::Scalar(0.0f, 0.0f, 1.0f));
	}
}

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

        points.emplace_back(static_cast<float>(x), static_cast<float>(y));
		drawPoints(mat, points, {});
	}
	else
    {
        hullCalculated = true;
		drawPoints(mat, points, CONVEX_HULL_METHOD(points));
	}

    cv::imshow("Convexhull", mat);
    cv::waitKey(0);
}

#define OFFSET(x) (x)
#define SCALE (3.0f)

void convexhull()
{
	points.emplace_back(Point(20, OFFSET(22)) * SCALE);
    points.emplace_back(Point(30, OFFSET(30)) * SCALE);
    points.emplace_back(Point(40, OFFSET(25)) * SCALE);
    points.emplace_back(Point(60, OFFSET(40)) * SCALE);
    points.emplace_back(Point(80, OFFSET(33)) * SCALE);
	points.emplace_back(Point(75, OFFSET(45)) * SCALE);
	points.emplace_back(Point(65, OFFSET(28)) * SCALE);
	points.emplace_back(Point(45, OFFSET(20)) * SCALE);

	cv::Mat mat = cv::Mat(600, 600, CV_32FC3);
    drawPoints(mat, points, {});

	cv::namedWindow("Convexhull", 1);
	cv::setMouseCallback("Convexhull", callback, nullptr);
	cv::imshow("Convexhull", mat);
	cv::waitKey(0);
}
