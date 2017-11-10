#include "intervaltree.h"

IntervalTree::IntervalTree(std::vector<Point> points, int start, int end) : points(std::move(points)), start(start), end(end)
{
	if (this->points.empty())
	{
		throw "Invalid leaf";
	}
	std::sort(this->points.begin(), this->points.end(), [](const Point& p1, const Point& p2) {
		return p1.y < p2.y;
	});
}
IntervalTree::~IntervalTree()
{
	for (auto& node : this->nodes)
	{
		delete node;
	}
}

std::vector<Point> IntervalTree::find(std::vector<Point> points, const cv::Rect& rectangle)
{
	if (points.empty()) return points;

	std::sort(this->points.begin(), this->points.end(), [](const Point& p1, const Point& p2) {
		return p1.x < p2.x;
	});

	// too right or too left
	if (points.front().x > rectangle.br().x ||
		points.back().x < rectangle.tl().x) return {};

	auto cmp = [](const cv::Point2i& rp, const Point& p) {
		return rp.x < p.x;
	};
	auto left = std::lower_bound(points.begin(), points.end(), rectangle.tl(), cmp);
	auto right = std::upper_bound(points.begin(), points.end(), rectangle.br(), cmp);

	int leftIndex = static_cast<int>(left - points.begin());
	int rightIndex = static_cast<int>(right - points.begin());

	std::vector<Point> result;
	this->find(points, rectangle, leftIndex, rightIndex, result);
	return result;
}

static bool matchesRange(int rangeStart, int rangeEnd, int treeStart, int treeEnd)
{
	return	treeStart >= rangeStart && treeStart < rangeEnd ||
			treeEnd > rangeStart && treeEnd <= rangeEnd;
}

void IntervalTree::find(const std::vector<Point>& points, const cv::Rect& rectangle, int start, int end, std::vector<Point>& result)
{
	// fully cover
	if (this->isLeaf() || this->start >= start && this->end <= end)
	{
		for (auto& p : this->points)
		{
			if (p.y > rectangle.tl().y) return;
			if (rectContains(rectangle, p))
			{
				result.push_back(p);
			}
		}
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			if (matchesRange(start, end, this->nodes[i]->start, this->nodes[i]->end))
			{
				this->nodes[i]->find(points, rectangle, start, end, result);
			}
		}
	}
}

bool IntervalTree::isLeaf() const
{
	return this->points.size() == 1;
}

std::unique_ptr<IntervalTree> IntervalTree::buildTree(std::vector<Point> points)
{
	std::sort(points.begin(), points.end(), [](const Point& p1, const Point& p2) {
		return p1.x < p2.x;
	});

	int start = 0;
	auto end = points.size();

	return std::unique_ptr<IntervalTree>(IntervalTree::buildTree(points, points, start, end));
}
IntervalTree* IntervalTree::buildTree(const std::vector<Point>& pointsSortX, std::vector<Point> pointsInner, int start, int end)
{
	IntervalTree* tree = new IntervalTree(pointsInner, start, end);
	if (pointsInner.size() < 2) return tree;

	int middle = (start + end) / 2;
	std::vector<Point> halves[2];
	halves[0].insert(halves[0].end(), pointsSortX.begin() + start, pointsSortX.begin() + middle);
	halves[1].insert(halves[1].end(), pointsSortX.begin() + middle, pointsSortX.begin() + end);

	tree->nodes[0] = IntervalTree::buildTree(pointsSortX, halves[0], start, middle);
	tree->nodes[1] = IntervalTree::buildTree(pointsSortX, halves[1], middle, end);

	return tree;
}
