#pragma once

#include <memory>
#include <vector>

#include "../algo.h"

class IntervalTree
{
public:
	static std::unique_ptr<IntervalTree> buildTree(std::vector<Point> points);

	IntervalTree(std::vector<Point> points, int start, int end);
	~IntervalTree();

	IntervalTree(const IntervalTree& other) = delete;
	IntervalTree(const IntervalTree&& other) = delete;
	IntervalTree operator=(const IntervalTree& other) = delete;

	std::vector<Point> find(std::vector<Point> points, const cv::Rect& rectangle);

private:
	static IntervalTree* buildTree(const std::vector<Point>& pointsSortX, std::vector<Point> pointsInner, int start, int end);

	void find(const std::vector<Point>& points, const cv::Rect& rectangle, int start, int end, std::vector<Point>& result);
	bool isLeaf() const;

	std::vector<Point> points;
	IntervalTree* nodes[2]{};

	int start;
	int end;
};
