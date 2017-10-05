#include "segment.h"

Segment::Segment(Line line, Point sweepCross) : line(line), sweepCross(sweepCross)
{

}
bool Segment::operator<(const Segment& other) const
{
    return this->sweepCross.y < other.sweepCross.y;
}
