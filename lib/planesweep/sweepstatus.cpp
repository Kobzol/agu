#include "sweepstatus.h"

void SweepStatus::insert(Segment* segment)
{
    this->lines.insert({segment});
}
Segment* SweepStatus::above(Segment* segment)
{
    auto bound = this->lines.upper_bound(segment);
    return bound == this->lines.end() ? nullptr : *bound;
}
Segment* SweepStatus::below(Segment* segment)
{
    auto bound = this->lines.lower_bound(segment);

    if (bound == this->lines.begin() || bound == this->lines.begin()) return nullptr;
    else return *(--bound);
}
void SweepStatus::remove(Segment* segment)
{
	auto it = this->lines.find(segment);
	if (it != this->lines.end())
	{
		this->lines.erase(it);
	}
}

void SweepStatus::exchange(Segment* segment1, Segment* segment2, const Point& crossPoint)
{
    this->remove(segment1);
    this->remove(segment2);

	/*segment1->sweepCross = crossPoint;
	segment2->sweepCross = crossPoint;
	segment1->sweepCross.y += 0.01f;*/
    std::swap(segment1->sweepCross, segment2->sweepCross);

    this->insert(segment1);
    this->insert(segment2);
}
