#include "triangletree.h"

#include <utility>

#include "../algo.h"

TriangleTree::TriangleTree(std::shared_ptr<Triangle> triangle): triangle(std::move(triangle))
{

}

bool TriangleTree::contains(const Point& point) const
{
    std::vector<Point> points;
    for (auto& v: this->triangle->vertices)
    {
        points.push_back(v->location);
    }
    return liesInsideTriangle(point, points);
}

bool TriangleTree::isLeaf() const
{
    return this->children.empty();
}

TriangleTree* TriangleTree::find(const Point& point)
{
    if (this->contains(point))
    {
        if (this->isLeaf()) return this;

        for (auto& child: this->children)
        {
            auto res = child->find(point);
            if (res != nullptr) return res;
        }

        throw "wrong tree structure";
    }

    return nullptr;
}
