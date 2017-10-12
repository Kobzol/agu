#include "draw.h"

void drawLines(cv::Mat& image, const std::vector<Line>& lines, const cv::Scalar &color)
{
    for (auto& line : lines)
    {
        cv::line(image, Point(line.first.x, image.rows - line.first.y), Point(line.second.x, image.rows - line.second.y), color, 1, CV_AA);
    }
}
void drawLines(cv::Mat& image, const std::vector<Point>& lines, const cv::Scalar &color)
{
    for (int i = 0; i < lines.size(); i++)
    {
        auto next = lines[(i + 1) % lines.size()];
        cv::line(image, Point(lines[i].x, image.rows - lines[i].y), Point(next.x, image.rows - next.y), color, 1, CV_AA);
    }
}

void drawPoints(cv::Mat& image, const std::vector<Point>& points, const cv::Scalar &color, float radius)
{
    for (auto& point : points)
    {
        cv::circle(image, Point(point.x, image.rows - point.y), static_cast<int>(radius), color, 1, CV_AA);
    }
}

void drawHull(cv::Mat& image, const std::vector<Point>& points, const std::vector<Point>& hull)
{
    for (int i = 0; i < points.size(); i++)
    {
        auto p = Point(points[i].x, image.rows - points[i].y);
        cv::circle(image, p, 2, cv::Scalar(1.0f, 1.0f, 1.0f));
        cv::putText(image, std::to_string(i), p + Point(0.2f, 0.0f), CV_FONT_HERSHEY_COMPLEX_SMALL,
                    0.5f, cv::Scalar(0.0f, 1.0f, 0.0f), 1, CV_AA);
    }

    drawLines(image, hull, cv::Scalar(0.0f, 0.0f, 1.0f));
}
