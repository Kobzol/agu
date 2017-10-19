#pragma once

#include <set>

#include "segment.h"
#include "segmentcomparer.h"

extern float SORT_X;

class SweepStatus
{
public:
    void insert(Segment* segment, const Point& location);
    Segment* above(Segment* segment, const Point& location);
    Segment* below(Segment* segment, const Point& location);
    void remove(Segment* segment, const Point& location);
    void exchange(Segment* segment1, Segment* segment2, const Point& crossPoint);

private:
    std::set<Segment*, SegmentComparer> lines;
};
