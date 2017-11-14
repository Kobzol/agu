#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

#include <algo.h>
#include <draw.h>
#include <triangulation/triangulation.h>

static std::vector<Point> points;
static bool triangulationCalculated = false;
static Planarmap map;
static int lastTriangleId = 0;
static cv::Mat mat = cv::Mat(600, 600, CV_32FC3);

#define OFFSET(x) (x)
#define SCALE (1.0f)

static void callback(int event, int x, int y, int flags, void* userdata)
{
    Point point(static_cast<float>(x), static_cast<float>(mat.rows - y));

    if (event == cv::EVENT_LBUTTONDOWN)
    {
        if (triangulationCalculated)
        {
            triangulationCalculated = false;
            points.clear();
        }
        points.push_back(point);

		mat = cv::Mat::zeros(600, 600, CV_32FC3);
        drawPoints(mat, points);
    }
    else if (event == cv::EVENT_RBUTTONDOWN)
    {
        triangulationCalculated = true;
        map = triangulate(points);

		mat = cv::Mat::zeros(600, 600, CV_32FC3);
        drawLines(mat, mapToLines(map));
		drawLines(mat, mapToVoronoi(map), cv::Scalar(0.0f, 0.0f, 1.0f));
    }
    else if (event == cv::EVENT_MOUSEMOVE)
    {
		if (triangulationCalculated)
		{
			points.resize(points.size() - 1);
			points.push_back(point);
			map = triangulate(points);

			mat = cv::Mat::zeros(600, 600, CV_32FC3);

			drawLines(mat, mapToLines(map));
			drawLines(mat, mapToVoronoi(map), cv::Scalar(0.0f, 0.0f, 1.0f));
		}

        /*if (triangulationCalculated)
        {
            auto triangles = map.getTriangles();
            if (!triangles.empty())
            {
                drawLines(mat, mapToLines(map));
                for (auto& t : triangles)
                {
                    if (liesInsideTriangle(point, { t->vertices[0]->location,
                                                    t->vertices[1]->location,
                                                    t->vertices[2]->location }))
                    {
                        if (t->id != lastTriangleId)
                        {
                            lastTriangleId = t->id;
                            std::cout << t->id << std::endl;
                        }

                        drawTriangle(mat, *t, cv::Scalar(0.0f, 0.0f, 1.0f));
                    }
                }

                cv::imshow("Triangulation", mat);
                cv::waitKey();
            }
        }*/
    }
}

void triangulation()
{
	cv::namedWindow("Triangulation", 1);
	cv::setMouseCallback("Triangulation", callback, nullptr);
	
	while (true)
	{
		cv::imshow("Triangulation", mat);
		if (cv::waitKey(10) == 27) break;	// escape
	}
}
