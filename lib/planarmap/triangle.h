#pragma once

#include <memory>
#include "vertex.h"

class Triangle
{
public:
    static int ID;

    int getPointIndex(std::shared_ptr<Vertex> vertex) const;
    int getThirdPointIndex(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2) const;

    std::shared_ptr<Vertex> vertices[3];
    std::shared_ptr<Triangle> neighbours[3];

    int id = ID++;
};

inline std::ostream& operator<<(std::ostream& os, const Triangle& t)
{
    os << "Triangle " << t.id << ", n: ";
    for (auto& neighbour : t.neighbours)
    {
        os << (neighbour ? neighbour->id : -1) << " ";
    }

    return os;
};
