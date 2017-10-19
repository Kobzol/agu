#pragma once

class Segment;

class SegmentComparer
{
public:
	bool operator()(const Segment* s1, const Segment* s2) const;
};
