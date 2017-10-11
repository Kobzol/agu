#include "planesweep.h"
#include "segment.h"
#include "event.h"
#include "sweepstatus.h"
#include "../algo.h"

#include <queue>
#include <functional>

#ifdef USE_STD_PRIORITY_QUEUE
    template <typename T>
    using PriorityQueue = std::priority_queue<T, std::vector<T>, std::greater<T>>;
#else
    #include "priority_queue.h"

#endif

static std::vector<Segment> createSegments(const std::vector<Line>& lines)
{
	std::vector<Segment> segments;

	for (auto& line : lines)
	{
		Segment segment = Segment(line, line.first);
		if (segment.line.first.x > segment.line.second.x)
		{
			std::swap(segment.line.first, segment.line.second);
			segment.sweepCross = segment.line.first;
		}

		segments.push_back(segment);
	}

	return segments;
}
static PriorityQueue<Event> createEventQueue(std::vector<Segment>& segments)
{
	PriorityQueue<Event> queue;
	for (auto& segment : segments)
	{
		queue.push(Event(segment.line.first, &segment, EventType::Start));
		queue.push(Event(segment.line.second, &segment, EventType::End));
	}
	return queue;
}

static void addIfIntersects(std::vector<Event>& events, Segment* above, Segment* below)
{
	if (above == nullptr || below == nullptr) return;

	Point intersect = lineIntersection(above->line, below->line);
	if (intersect != INVALID_POINT)
	{
		events.emplace_back(above, below, intersect);
	}
}

using EventFn = void (*)(Event& event, PriorityQueue<Event>& queue, SweepStatus& status, std::vector<Event>& events);
static void handleStartPoint(Event& event, PriorityQueue<Event>& queue, SweepStatus& status, std::vector<Event>& events)
{
	status.insert(event.segment1);

	auto above = status.above(event.segment1);
	auto below = status.below(event.segment1);

	addIfIntersects(events, above, event.segment1);
	addIfIntersects(events, event.segment1, below);
}
static void handleEndPoint(Event& event, PriorityQueue<Event>& queue, SweepStatus& status, std::vector<Event>& events)
{
	auto above = status.above(event.segment1);
	auto below = status.below(event.segment1);

	addIfIntersects(events, above, below);

	status.remove(event.segment1);
}
static void handleCrossPoint(Event& event, PriorityQueue<Event>& queue, SweepStatus& status, std::vector<Event>& events)
{
	auto s1 = event.segment1;
	auto s2 = event.segment2;
	auto s3 = status.above(s1);
	auto s4 = status.below(s2);

	addIfIntersects(events, s3, s2);
	addIfIntersects(events, s1, s4);

	status.exchange(s1, s2, event.point);
}
static EventFn selectFn(EventType type)
{
	switch (type)
	{
		case EventType::Start:			return handleStartPoint;
		case EventType::End:			return handleEndPoint;
		case EventType::Intersection:	return handleCrossPoint;
	}

	return nullptr;
}

// TODO: handle special cases
std::vector<Point> bentleyOttmann(const std::vector<Line>& lines)
{
	SweepStatus status;
	std::vector<Event> events;
	std::vector<Point> points;

	auto segments = createSegments(lines);
	auto queue = createEventQueue(segments);

	while (!queue.empty())
	{
		Event event = queue.top();
        queue.pop();

		selectFn(event.type)(event, queue, status, events);

		for (auto& ev : events)
		{
#ifndef USE_STD_PRIORITY_QUEUE
            if (!queue.contains(ev))
#endif
            {
                queue.push(ev);
                points.push_back(ev.point);
            }
		}
		events.clear();
	}

	return points;
}
