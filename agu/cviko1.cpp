#include <iostream>

#include <opencv2/world.hpp>
#include <opencv2/highgui.hpp>

#include "lib/polygon.h"
#include "lib/algo.h"

struct UserData
{
	UserData(cv::Mat* mat, Polygon* polygon) : mat(mat), polygon(polygon)
	{

	}

	cv::Mat* mat;
	Polygon* polygon;
	int counter = 0;
	Point from;
	Point to;
	Point target;
};

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	UserData* data = static_cast<UserData*>(userdata);

	if (event == cv::EVENT_LBUTTONDOWN)
	{
		/*if (data->counter == 0)
		{
			data->from = Point(x, y);
		}
		else if (data->counter == 1)
		{
			data->to = Point(x, y);
		}
		else
		{
			data->target = Point(x, y);
			*data->mat = cv::Mat::zeros(cv::Size(data->mat->rows, data->mat->cols), data->mat->type());

			std::cerr << testPointRight(data->target, data->from, data->to) << std::endl;
		}

		data->counter = (data->counter + 1) % 3;

		cv::line(*data->mat, data->from, data->to, cv::Scalar(0.0, 1.0, 0.0));
		cv::circle(*data->mat, data->target, 5, cv::Scalar::all(1.0));*/

		std::cout << (data->polygon->test((float) x, (float) y, *data->mat) ? "In" : "Out") << std::endl;
		cv::imshow("Polygon", *data->mat);
	}
}

int main()
{
	cv::Mat mat = cv::Mat(600, 600, CV_32FC3);
	Polygon polygon = Polygon::generate(0.0f, 300.0f, 6);
	polygon.draw(mat);

	UserData data(&mat, &polygon);

	cv::namedWindow("Polygon", 1);
	cv::setMouseCallback("Polygon", CallBackFunc, &data);
	cv::imshow("Polygon", mat);
	cv::waitKey(0);

	return 0;
}
