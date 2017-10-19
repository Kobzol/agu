#pragma once

#include <set>

#include "segment.h"

class SegmentComparer
{
public:
    bool operator()(const Segment* s1, const Segment* s2) const
    {
        return s1->sweepCross.y < s2->sweepCross.y;
    }
};

class SweepStatus
{
public:
    void insert(Segment* segment);
    Segment* above(Segment* segment);
    Segment* below(Segment* segment);
    void remove(Segment* segment);
    void exchange(Segment* segment1, Segment* segment2, const Point& crossPoint);

private:
    std::set<Segment*, SegmentComparer> lines;
};
