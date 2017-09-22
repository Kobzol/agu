#include <iostream>

#include <opencv2/world.hpp>
#include <opencv2/highgui.hpp>

#include "lib/polygon.h"
#include "lib/algo.h"

struct UserData
{
	cv::Mat* mat;
	Polygon* polygon;
};

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	UserData* data = static_cast<UserData*>(userdata);

	if (event == cv::EVENT_LBUTTONDOWN)
	{
		std::cout << (data->polygon->test((float) x, (float) y, *data->mat) ? "In" : "Out") << std::endl;
		cv::imshow("Polygon", *data->mat);
	}
}

int main()
{
	cv::Mat mat = cv::Mat(600, 600, CV_32FC3);
	Polygon polygon = Polygon::generate(0.0f, 300.0f, 6);
	polygon.draw(mat);

	UserData data = { &mat, &polygon };

	/*Point target(10, 10);
	Point from(3, 500);
	Point to(3, 0);

	std::cerr << testPointRight(target, from, to) << std::endl;
	cv::line(mat, from, to, cv::Scalar::all(1.0f));
	cv::circle(mat, target, 10, cv::Scalar(0.0, 0.0, 1.0));*/

	cv::namedWindow("Polygon", 1);
	cv::setMouseCallback("Polygon", CallBackFunc, &data);
	cv::imshow("Polygon", mat);
	cv::waitKey(0);

	return 0;
}
