#include <iostream>
#include <fstream>

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
		endpoints[pointIndex] = Point(static_cast<float>(x), static_cast<float>(600 - y));
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

		std::ofstream fs("lines.txt");
		for (auto& line : lines)
		{
			fs << "lines.emplace_back(Point(" << line.first.x << ", OFFSET(" << line.first.y << ")), Point(" << line.second.x << ", OFFSET(" << line.second.y << ")));" << std::endl;
		}
		fs.close();

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
	/*lines.emplace_back(Point(40, OFFSET(485)), Point(151, OFFSET(586)));
	lines.emplace_back(Point(77, OFFSET(459)), Point(159, OFFSET(552)));
	lines.emplace_back(Point(106, OFFSET(425)), Point(151, OFFSET(430)));
	lines.emplace_back(Point(110, OFFSET(479)), Point(150, OFFSET(381)));*/

	/*lines.emplace_back(Point(53.372242185755624, 173.39620884287083), Point(387.8525818239285, 398.87254440778935));
	lines.emplace_back(Point(80.31526735980506, 339.67057983427924), Point(433.19719584798537, 147.01092888548328));
	lines.emplace_back(Point(125.66972270166728, 55.743054445253435), Point(232.3797092128601, 192.28367154112865));
	lines.emplace_back(Point(159.15549167649306, 153.02062212796213), Point(443.29412264971825, 100.62110007192734));
	lines.emplace_back(Point(163.96688762758603, 206.99734365408403), Point(415.07301876538446, 38.55247900773355));
	lines.emplace_back(Point(253.00029825515966, 330.25595259609173), Point(346.3358935566799, 361.4159712080646));*/

	cv::Mat mat = cv::Mat(600, 600, CV_32FC3);
	drawAll(mat, lines);

	cv::namedWindow("Bentley-Ottmann", 1);
	cv::setMouseCallback("Bentley-Ottmann", callback, nullptr);
	cv::imshow("Bentley-Ottmann", mat);
	cv::waitKey(0);
}
