#include "planesweep.h"
#include "segment.h"
#include "event.h"
#include "sweepstatus.h"
#include "../algo.h"

#include <queue>
#include <functional>
#include <set>

template <typename T>
using PriorityQueue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

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

using EventFn = void (*)(Event& event, PriorityQueue<Event>& queue, SweepStatus& status, std::vector<Event>& events);
static void handleStartPoint(Event& event, PriorityQueue<Event>& queue, SweepStatus& status, std::vector<Event>& events)
{
	status.insert(event.segment1);

	auto above = status.above(event.segment1);
	auto below = status.below(event.segment1);

	if (above != nullptr && lineIntersects(above->line, event.segment1->line))
	{
		events.push_back(Event(above, event.segment1, lineIntersection(above->line, event.segment1->line)));
	}
	if (below != nullptr && lineIntersects(event.segment1->line, below->line))
	{
		events.push_back(Event(event.segment1, below, lineIntersection(event.segment1->line, below->line)));
	}
}
static void handleEndPoint(Event& event, PriorityQueue<Event>& queue, SweepStatus& status, std::vector<Event>& events)
{
	auto above = status.above(event.segment1);
	auto below = status.below(event.segment1);

	if (above != nullptr && below != nullptr && lineIntersects(above->line, below->line))
	{
		events.push_back(Event(above, below, lineIntersection(above->line, below->line)));
	}

	status.remove(event.segment1);
}
static void handleCrossPoint(Event& event, PriorityQueue<Event>& queue, SweepStatus& status, std::vector<Event>& events)
{
	auto s1 = event.segment1;
	auto s2 = event.segment2;
	auto s3 = status.above(s1);
	auto s4 = status.below(s2);

	if (s3 != nullptr && lineIntersects(s3->line, s2->line))
	{
		events.push_back(Event(s3, s2, lineIntersection(s3->line, s2->line)));
	}
	if (s4 != nullptr && lineIntersects(s1->line, s4->line))
	{
		events.push_back(Event(s1, s4, lineIntersection(s1->line, s4->line)));
	}

	status.exchange(s1, s2);
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
			queue.push(ev);
			points.push_back(ev.point);
		}
		events.clear();
	}

	return points;
}
