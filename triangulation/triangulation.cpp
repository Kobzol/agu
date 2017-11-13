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

#define OFFSET(x) (x)
#define SCALE (1.0f)

static void callback(int event, int x, int y, int flags, void* userdata)
{
    cv::Mat mat = cv::Mat(600, 600, CV_32FC3);
    Point point(static_cast<float>(x), static_cast<float>(mat.rows - y));

    if (event == cv::EVENT_LBUTTONDOWN)
    {
        if (triangulationCalculated)
        {
            triangulationCalculated = false;
            points.clear();
        }
        points.push_back(point);
        drawPoints(mat, points);

        cv::imshow("Triangulation", mat);
        cv::waitKey();
    }
    else if (event == cv::EVENT_RBUTTONDOWN)
    {
        triangulationCalculated = true;
        map = triangulate(points);
        drawLines(mat, mapToLines(map));
        cv::imshow("Triangulation", mat);
        cv::waitKey();

    }
    else if (event == cv::EVENT_MOUSEMOVE)
    {
        if (triangulationCalculated)
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
        }
    }
}

void triangulation()
{
    cv::Mat mat = cv::Mat(600, 600, CV_32FC3);

	cv::namedWindow("Triangulation", 1);
	cv::setMouseCallback("Triangulation", callback, nullptr);
	cv::imshow("Triangulation", mat);

	cv::waitKey();
}
