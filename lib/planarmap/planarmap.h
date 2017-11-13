#pragma once

#include <memory>
#include "vertex.h"
#include "triangle.h"

class Planarmap
{
public:
    std::shared_ptr<Vertex> addVertex(const Point& point);

    std::shared_ptr<Triangle> addTriangle(const std::vector<std::shared_ptr<Vertex>>& points);
    void removeTriangle(std::shared_ptr<Triangle> triangle);

    std::vector<std::shared_ptr<Vertex>> getVertices() const;
    std::vector<std::shared_ptr<Triangle>> getTriangles() const;

private:
    std::shared_ptr<Triangle> addTriangle();

    std::vector<std::shared_ptr<Vertex>>     vertices;
    std::vector<std::shared_ptr<Triangle>>   triangles;
};
