#include "segmentcomparer.h"
#include "../algo.h"
#include "sweepstatus.h"

bool SegmentComparer::operator()(const Segment* s1, const Segment* s2) const
{
    float a, b;
    float c, d;

    getLineSlope(s1->line, a, b);
    getLineSlope(s2->line, c, d);

    float y1 = a * SORT_X + b;
    float y2 = c * SORT_X + d;

    return y1 < y2;
}
