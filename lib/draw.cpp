#include "draw.h"

void drawLines(cv::Mat& image, const std::vector<Line>& lines, const cv::Scalar &color)
{
    for (auto& line : lines)
    {
        cv::line(image, drawOffset(line.first), Point(line.second.x, image.rows - line.second.y), color, 1, CV_AA);
    }
}
void drawLines(cv::Mat& image, const std::vector<Point>& lines, const cv::Scalar &color)
{
    for (int i = 0; i < lines.size(); i++)
    {
        auto next = lines[(i + 1) % lines.size()];
        cv::line(image, drawOffset(lines[i]), drawOffset(next), color, 1, CV_AA);
    }
}

void drawPoints(cv::Mat& image, const std::vector<Point>& points, const cv::Scalar &color, float radius)
{
    for (auto& point : points)
    {
        cv::circle(image, drawOffset(point), static_cast<int>(radius), color, 1, CV_AA);
    }
}

void drawHull(cv::Mat& image, const std::vector<Point>& points, const std::vector<Point>& hull)
{
    for (int i = 0; i < points.size(); i++)
    {
        auto p = drawOffset(points[i]);
        cv::circle(image, p, 2, cv::Scalar(1.0f, 1.0f, 1.0f));
        cv::putText(image, std::to_string(i), p + Point(0.2f, 0.0f), CV_FONT_HERSHEY_COMPLEX_SMALL,
                    0.5f, cv::Scalar(0.0f, 1.0f, 0.0f), 1, CV_AA);
    }

    drawLines(image, hull, cv::Scalar(0.0f, 0.0f, 1.0f));
}

Point drawOffset(const Point& p)
{
    return Point(p.x, 600 - p.y);
}

void drawTriangle(cv::Mat& mat, const Triangle& triangle, const cv::Scalar& color)
{
    std::vector<Point> points;
    Point center(0, 0);
    for (auto& v: triangle.vertices)
    {
        points.push_back(v->location);
        center += v->location;
    }
    center /= 3;

    drawLines(mat, points, color);
    cv::putText(mat, std::to_string(triangle.id), drawOffset(center), CV_FONT_HERSHEY_COMPLEX_SMALL,
                0.6f, color);
}

void drawPlanarmap(cv::Mat& mat, const Planarmap& planarmap)
{
    for (auto& t : planarmap.getTriangles())
    {
        drawTriangle(mat, *t);
    }
}
