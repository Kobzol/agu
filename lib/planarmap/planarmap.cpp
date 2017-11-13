#include "planarmap.h"
#include "../draw.h"

std::shared_ptr<Vertex> Planarmap::addVertex(const Point& point)
{
    auto vertex = std::make_shared<Vertex>(point);
    this->vertices.push_back(vertex);
    return vertex;
}

std::shared_ptr<Triangle> Planarmap::addTriangle(const std::vector<std::shared_ptr<Vertex>>& points)
{
    auto triangle = this->addTriangle();
    for (int i = 0; i < 3; i++)
    {
        triangle->vertices[i] = points[i];
    }

    return triangle;
}

std::shared_ptr<Triangle> Planarmap::addTriangle()
{
    auto triangle = std::make_shared<Triangle>();
    this->triangles.push_back(triangle);
    return triangle;
}
std::vector<std::shared_ptr<Vertex>> Planarmap::getVertices() const
{
    return this->vertices;
}
std::vector<std::shared_ptr<Triangle>> Planarmap::getTriangles() const
{
    return this->triangles;
}

void Planarmap::removeTriangle(std::shared_ptr<Triangle> triangle)
{
    auto it = std::find(this->triangles.begin(), this->triangles.end(), triangle);
    assert(it != this->triangles.end());
    this->triangles.erase(it);
}
