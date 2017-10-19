#pragma once

#include "segment.h"

enum class EventType
{
    Start = 1,
    End = 2,
    Intersection = 3
};

class Event
{
public:
    Event(Point point, Segment* segment1, EventType type);
    Event(Segment* segment1, Segment* segment2, Point point);

    bool operator<(const Event& other) const;
    bool operator>(const Event& other) const;

    EventType type;

    Point point;
    Segment* segment1;
    Segment* segment2;
};
