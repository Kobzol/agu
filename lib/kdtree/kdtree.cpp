#include "kdtree.h"
#include "../algo.h"

static KDTree* buildTree(const Point& point, Axis axis, const std::vector<Point>& points)
{
    auto* tree = new KDTree(point, axis);
    if (points.empty()) return tree;

    std::vector<Point> halves[2];
    if (axis == Axis::Vertical)
    {
        for (auto& p : points)
        {
            if (p.x < point.x) halves[0].push_back(p);
            else halves[1].push_back(p);
        }

        /*for (int i = 0; i < 2; i++)
        {
            std::sort(halves[i].begin(), halves[i].end(), [](const Point& p1, const Point& p2) {
                return p1.y < p2.y;
            });
        }*/
    }
    else
    {
        for (auto& p : points)
        {
            if (p.y < point.y) halves[0].push_back(p);
            else halves[1].push_back(p);
        }
        /*for (int i = 0; i < 2; i++)
        {
            std::sort(halves[i].begin(), halves[i].end(), [](const Point& p1, const Point& p2) {
                return p1.x < p2.x;
            });
        }*/
    }

    for (int i = 0; i < 2; i++)
    {
		if (!halves[i].empty())
		{
			size_t index = halves[i].size() / 2;
			Point point = halves[i][index];
			std::swap(halves[i][index], halves[i][halves[i].size() - 1]);
			halves[i].resize(halves[i].size() - 1);
			tree->nodes[i] = buildTree(point, (Axis)(1 - (int)axis), halves[i]);
		}
    }

    return tree;
}

static std::pair<bool, bool> checkIntersection(Axis axis, const cv::Rect& rectangle, const Point& point)
{
	if (axis == Axis::Vertical)
	{
		return std::make_pair(rectangle.x < point.x, (rectangle.x + rectangle.width) > point.x);
	}
	else return std::make_pair((rectangle.y - rectangle.height) < point.y, rectangle.y > point.y);
}

std::unique_ptr<KDTree> KDTree::buildTree(std::vector<Point> points)
{
    auto top = findExtremePointY<false>(points);
	auto point = points[top];

	std::swap(points[top], points.back());
	points.resize(points.size() - 1);

    return std::unique_ptr<KDTree>(::buildTree(point, Axis::Vertical, points));
}

KDTree::KDTree(Point point, Axis axis): point(point), axis(axis)
{

}
KDTree::~KDTree()
{
    for (auto& node : this->nodes)
    {
        delete node;
    }
}

bool KDTree::is_leaf() const
{
    return this->nodes[0] == nullptr && this->nodes[1] == nullptr;
}

std::vector<Point> KDTree::find(const std::vector<Point>& allPoints, const cv::Rect& rectangle)
{
	std::vector<Point> points;
	this->find(allPoints, rectangle, Axis::Vertical, points);
	return points;
}

void KDTree::find(const std::vector<Point>& allPoints, const cv::Rect& rectangle, Axis axis, std::vector<Point>& points)
{
	if (rectContains(rectangle, this->point))
	{
		points.push_back(this->point);
	}

	if (this->is_leaf()) return;

	auto intersections = checkIntersection(axis, rectangle, this->point);
	if (this->nodes[0] != nullptr && intersections.first)
	{
		this->nodes[0]->find(allPoints, rectangle, (Axis)(1 - (int) axis), points);
	}
	if (this->nodes[1] != nullptr && intersections.second)
	{
		this->nodes[1]->find(allPoints, rectangle, (Axis)(1 - (int)axis), points);
	}
}
