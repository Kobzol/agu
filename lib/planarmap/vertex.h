#pragma once

#include <memory>
#include "../common.h"

class Vertex
{
public:
    explicit Vertex(const Point& location);

    Point location;
};
