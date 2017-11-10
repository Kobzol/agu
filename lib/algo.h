#pragma once

#include "polygon.h"
#include "common.h"

#define INVALID_POINT (Point(-1, -1))

cv::Vec2f toVec(const Point& point);

bool lineIntersects(const Line& line1, const Line& line2);
Point lineIntersection(const Line& line1, const Line& line2);
Point bisectorIntersection(const Line& line1, const Line& line2);
float linePointDistance(const Line& line, const Point& point);
void getLineSlope(const Line& line, float& slopeA, float& slopeC);

int testPointCCW(const Point& p, const Point& p1, const Point& p2);
bool testPointRight(const Point& point, const Point& from, const Point& to);
bool liesInsideTriangle(const Point& point, const std::vector<Point>& triangle);
bool rectContains(const cv::Rect& rectangle, const cv::Point& point);

Point weightCenter(const std::vector<Point>& points);

void dumpPoints(std::ostream& os, const std::vector<Point>& points);
void dumpLines(std::ostream& os, const std::vector<Line>& lines);

template <bool smallest = true>
inline int findExtremePointX(const std::vector<Point>& points)
{
    int extreme = 0;
    size_t size = points.size();
    for (int i = 1; i < size; i++)
    {
        if (smallest)
        {
            if (points[i].x < points[extreme].x)
            {
                extreme = i;
            }
        }
        else
        {
            if (points[i].x > points[extreme].x)
            {
                extreme = i;
            }
        }
    }

    return extreme;
}

template <bool smallest = true>
inline int findExtremePointY(const std::vector<Point>& points)
{
    int extreme = 0;
    size_t size = points.size();
    for (int i = 1; i < size; i++)
    {
        if (smallest)
        {
            if (points[i].y < points[extreme].y)
            {
                extreme = i;
            }
        }
        else
        {
            if (points[i].y > points[extreme].y)
            {
                extreme = i;
            }
        }
    }

    return extreme;
}
