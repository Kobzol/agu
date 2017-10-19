#pragma once

#include <set>
#include "../common.h"
#include "segmentcomparer.h"

class Segment
{
public:
    Segment(Line line, Point sweepCross);

    bool operator<(const Segment& other) const;
    Line line;
    Point sweepCross;
    std::set<Segment*, SegmentComparer>::iterator iterator;
};
