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

    bool isLeaf() const;
	std::vector<Point> find(const std::vector<Point>& originalPoints, const cv::Rect& rectangle);

private:
	static KDTree* buildTree(const Point& point, Axis axis, const std::vector<Point>& points);

	void find(const cv::Rect& rectangle, Axis axis, std::vector<Point>& points);

	Point point;
	Axis axis = Axis::Horizontal;
	KDTree* nodes[2]{};
};
