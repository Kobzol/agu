#include "event.h"

Event::Event(Point point, Segment* segment, EventType type)
        : type(type), point(point), segment1(segment)
{

}
Event::Event(Segment* segment1, Segment* segment2, Point point)
        : type(EventType::Intersection), point(point), segment1(segment1), segment2(segment2)
{

}

bool Event::operator>(const Event& other) const
{
    return this->point.x > other.point.x;
}
