#pragma once

#include "../polygon.h"

class Segment
{
public:
    Segment(Line line, Point sweepCross);

    bool operator<(const Segment& other) const;
    Line line;
    Point sweepCross;
};
