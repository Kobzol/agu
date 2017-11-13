#include "triangle.h"

int Triangle::ID = 1;

int Triangle::getThirdPointIndex(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2) const
{
    for (int i = 0; i < 3; i++)
    {
        auto addr = this->vertices[i].get();
        if (addr != v1.get() && addr != v2.get())
        {
            return i;
        }
    }

    assert(false);
    return -1;
}

int Triangle::getPointIndex(std::shared_ptr<Vertex> vertex) const
{
    for (int i = 0; i < 3; i++)
    {
        auto addr = this->vertices[i].get();
        if (addr == vertex.get())
        {
            return i;
        }
    }

    return -1;
}
