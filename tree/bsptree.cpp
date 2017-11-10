#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>

#include <draw.h>
#include <tree/bsptree.h>

#define OFFSET(x) (x)
#define SCALE (1.0f)
using Tree = BspTree;

static std::vector<Line> lines;
static int pointIndex = 0;
static Point endpoints[2];
static std::unique_ptr<Tree> tree;

static void callback(int event, int x, int y, int flags, void* userdata)
{
    if (event != cv::EVENT_LBUTTONDOWN && event != cv::EVENT_RBUTTONDOWN) return;

    cv::Mat image = cv::Mat(600, 600, CV_32FC3);
    Point point(static_cast<float>(x), static_cast<float>(image.rows - y));

    if (event == cv::EVENT_LBUTTONDOWN)
    {
        endpoints[pointIndex] = point;
        pointIndex = (pointIndex + 1) % 2;

        if (pointIndex == 0)
        {
            lines.emplace_back(endpoints[0], endpoints[1]);
        }

		drawLines(image, lines);
		cv::imshow("BSPTree", image);
		cv::waitKey();
    }
	else if (event == cv::EVENT_RBUTTONDOWN)
	{
		drawPoints(image, { point }, cv::Scalar(0.0f, 1.0f, 0.0f));
		tree->paint(point, [&image](const Tree& tree) {
			drawLines(image, { tree.line });
			cv::imshow("Test", image);
			cv::waitKey();
		});
	}
}

void bsptree()
{
	lines.emplace_back(Point(321, OFFSET(514)), Point(364, OFFSET(379)));
	lines.emplace_back(Point(364, OFFSET(379)), Point(136, OFFSET(429)));
	lines.emplace_back(Point(136, OFFSET(429)), Point(197, OFFSET(507)));
	lines.emplace_back(Point(197, OFFSET(507)), Point(266, OFFSET(463)));
	lines.emplace_back(Point(266, OFFSET(463)), Point(321, OFFSET(514)));

    cv::Mat mat = cv::Mat(600, 600, CV_32FC3);
    drawLines(mat, lines);

    cv::namedWindow("BSPTree", 1);
    cv::setMouseCallback("BSPTree", callback, nullptr);
    cv::imshow("BSPTree", mat);

    tree = Tree::buildTree(lines);

    cv::waitKey();
}
