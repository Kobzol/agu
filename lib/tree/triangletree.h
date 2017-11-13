#pragma once

#include <memory>

#include "../common.h"
#include "../planarmap/vertex.h"
#include "../planarmap/triangle.h"

class TriangleTree
{
public:
    explicit TriangleTree(std::shared_ptr<Triangle> triangle);

    bool contains(const Point& point) const;
    bool isLeaf() const;

    TriangleTree* find(const Point& point);

    std::shared_ptr<Triangle> triangle;
    std::vector<std::shared_ptr<TriangleTree>> children;
};
