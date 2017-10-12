#pragma once


#include "../common.h"

#include <vector>
#include <memory>

enum class Axis
{
    Horizontal = 0,
    Vertical = 1
};

class KDTree
{
public:
    static std::unique_ptr<KDTree> buildTree(std::vector<Point> points);

    KDTree(Point point, Axis axis);
    ~KDTree();

    KDTree(const KDTree& other) = delete;
    KDTree(const KDTree&& other) = delete;
    KDTree operator=(const KDTree& other) = delete;

    bool is_leaf() const;
	std::vector<Point> find(const std::vector<Point>& allPoints, const cv::Rect& rectangle);

	int getPointCount()
	{
		if (this->is_leaf()) return 1;

		int count = 0;
		if (this->nodes[0] != nullptr)
		{
			count += this->nodes[0]->getPointCount();
		}
		if (this->nodes[1] != nullptr)
		{
			count += this->nodes[1]->getPointCount();
		}

		return 1 + count;
	}

    Point point;
    Axis axis = Axis::Horizontal;
    KDTree* nodes[2]{ };

private:
	void find(const std::vector<Point>& allPoints, const cv::Rect& rectangle, Axis axis, std::vector<Point>& points);
};
