#include "sweepstatus.h"

float SORT_X = 0;

void SweepStatus::insert(Segment* segment, const Point& location)
{
	SORT_X = location.x;
    this->lines.insert({segment});
}
Segment* SweepStatus::above(Segment* segment, const Point& location)
{
	SORT_X = location.x;
    auto bound = this->lines.upper_bound(segment);
    return bound == this->lines.end() ? nullptr : *bound;
}
Segment* SweepStatus::below(Segment* segment, const Point& location)
{
	SORT_X = location.x;
    auto bound = this->lines.lower_bound(segment);

    if (bound == this->lines.begin() || bound == this->lines.begin()) return nullptr;
    else return *(--bound);
}
void SweepStatus::remove(Segment* segment, const Point& location)
{
	SORT_X = location.x;
	auto it = this->lines.find(segment);
	if (it != this->lines.end())
	{
		this->lines.erase(it);
	}
}

void SweepStatus::exchange(Segment* segment1, Segment* segment2, const Point& crossPoint)
{
	SORT_X = crossPoint.x;
    this->remove(segment1, crossPoint);
    this->remove(segment2, crossPoint);

	/*segment1->sweepCross = crossPoint;
	segment2->sweepCross = crossPoint;
	segment1->sweepCross.y += 0.01f;*/
    std::swap(segment1->line, segment2->line);

    this->insert(segment1, crossPoint);
    this->insert(segment2, crossPoint);
}
