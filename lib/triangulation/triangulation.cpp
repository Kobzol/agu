#include "triangulation.h"

#include "../tree/triangletree.h"
#include "../draw.h"
#include "../algo.h"

//#define DRAW

static bool legalizeEdge(const std::shared_ptr<Triangle>& t1, const std::shared_ptr<Triangle>& t2,
                         int piIndex, int pjIndex)
{
    auto pi = t1->vertices[piIndex];
    auto pj = t1->vertices[pjIndex];

    int plIndex = t1->getThirdPointIndex(pi, pj);
    auto pl = t1->vertices[plIndex];

    int pkIndex = t2->getThirdPointIndex(pi, pj);
    auto pk = t2->vertices[pkIndex];

    Point center;
    double radius;

    createCircle(pi->location, pk->location, pj->location, center, radius);
    if (liesInsideCircle(pl->location, center, radius))
    {
        auto t3 = t1->neighbours[pjIndex];
        auto t5 = t2->neighbours[t2->getPointIndex(pi)];

        t1->neighbours[piIndex] = t5;
        t1->neighbours[pjIndex] = t2;

        if (t5)
        {
            t5->neighbours[t5->getPointIndex(pk)] = t1;
        }

        t2->neighbours[t2->getPointIndex(pi)] = t1;
        t2->neighbours[t2->getPointIndex(pj)] = t3;

        if (t3)
        {
            t3->neighbours[t3->getPointIndex(pl)] = t2;
        }

        t1->vertices[pjIndex] = pk;
        t2->vertices[t2->getPointIndex(pi)] = pl;

        return true;
    }

    return false;
}
static bool legalizeTriangle(const std::shared_ptr<Triangle>& triangle)
{
    for (int i = 0; i < 3; i++)
    {
        auto neighbor = triangle->neighbours[i];
        if (neighbor && legalizeEdge(triangle, neighbor, i, (i + 1) % 3))
        {
            std::cout << "Legalized " << triangle->id << " with " << neighbor->id << std::endl;
            return true;
        }
    }

    return false;
}
static void legalizeTriangles(Planarmap& map)
{
    auto triangles = map.getTriangles();

#ifdef DRAW
    int legCounter = 0;
#endif
    while (true)
    {
        bool legalized = false;
        for (auto& triangle : triangles)
        {
            if (legalizeTriangle(triangle))
            {
#ifdef DRAW
                for (auto& t: triangles)
                {
                    std::cout << *t << std::endl;
                }
                std::cout << std::endl;

                cv::Mat mat = cv::Mat::zeros(600, 600, CV_32FC3);
                drawTriangles(mat, map);
                cv::imshow("legalized" + std::to_string(legCounter++), mat);
                cv::waitKey();
#endif

                legalized = true;
                break;
            }
        }

        if (!legalized) break;
    }
}

static void addPointToTree(Planarmap& map, TriangleTree& tree, const Point& point)
{
#ifdef DRAW
    static int counter = 0;
#endif

    auto parent = tree.find(point);
    assert(parent);
    assert(parent->children.empty());

    auto triangle = parent->triangle;
    auto vertex = map.addVertex(point);

    std::vector<std::shared_ptr<Triangle>> triangles {
        map.addTriangle({ triangle->vertices[0], triangle->vertices[1], vertex }),
        map.addTriangle({ triangle->vertices[1], triangle->vertices[2], vertex }),
        map.addTriangle({ triangle->vertices[2], triangle->vertices[0], vertex })
    };

    for (int i = 0; i < 3; i++)
    {
        triangles[i]->neighbours[0] = triangle->neighbours[i];
        triangles[i]->neighbours[1] = triangles[(i + 1) % 3];
        triangles[i]->neighbours[2] = triangles[(i + 2) % 3];
    }

    for (int i = 0; i < 3; i++)
    {
        if (triangle->neighbours[i])
        {
            int index = triangle->neighbours[i]->getPointIndex(triangle->vertices[(i + 1) % 3]);
            triangle->neighbours[i]->neighbours[index] = triangles[i];
        }
    }

    for (auto& t: triangles)
    {
        /*cv::Mat mat = cv::Mat::zeros(600, 600, CV_32FC3);
        drawTriangle(mat, *t);
        cv::imshow("Triangle" + std::to_string(counter++), mat);
        cv::waitKey();*/

        auto subtree = std::make_shared<TriangleTree>(t);
        parent->children.push_back(subtree);
    }

    map.removeTriangle(triangle);
}

static void joinTriangles(const std::vector<std::shared_ptr<Triangle>>& triangles)
{
    for (int i = 0; i < triangles.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            auto v1 = triangles[i]->vertices[j];
            auto v2 = triangles[i]->vertices[(j + 1) % 3];

            int t = 0;
            for (; t < triangles.size(); t++)
            {
                if (i == t) continue;
                if (triangles[t]->getPointIndex(v1) != -1 &&
                    triangles[t]->getPointIndex(v2) != -1)
                {
                    break;
                }
            }

            if (t < triangles.size())
            {
                triangles[i]->neighbours[j] = triangles[t];
            }
        }
    }
}
static void testRightAngle()
{
    Planarmap map;

    int startX = 40;
    int startY = 40;

    std::vector<std::shared_ptr<Vertex>> vertices {
            map.addVertex(Point(startX - 40, startY + 40)), // pi
            map.addVertex(Point(startX + 40, startY)), // pk
            map.addVertex(Point(startX + 40, startY + 80)), // pl
            map.addVertex(Point(startX + 120, startY + 40)), // pj

            map.addVertex(Point(startX, startY)), // 4 (left down)
            map.addVertex(Point(startX, startY + 80)), // 5
            map.addVertex(Point(startX + 80, startY + 80)), // 6
            map.addVertex(Point(startX + 80, startY))  // 7
    };

    auto t1 = map.addTriangle({ vertices[0], vertices[3], vertices[2] });
    auto t2 = map.addTriangle({ vertices[0], vertices[1], vertices[3] });
    auto t3 = map.addTriangle({ vertices[2], vertices[3], vertices[6] });
    auto t4 = map.addTriangle({ vertices[5], vertices[0], vertices[2] });
    auto t5 = map.addTriangle({ vertices[1], vertices[0], vertices[4] });
    auto t6 = map.addTriangle({ vertices[1], vertices[7], vertices[3] });

    joinTriangles(map.getTriangles());

    for (auto& t: map.getTriangles())
    {
        std::cout << *t << std::endl;
    }

    cv::Mat mat = cv::Mat(600, 600, CV_32FC3);
    drawPlanarmap(mat, map);
    cv::imshow("t", mat);
    cv::waitKey();

    legalizeTriangles(map);

    std::cout << std::endl;
    for (auto& t: map.getTriangles())
    {
        std::cout << *t << std::endl;
    }

    mat = cv::Mat(600, 600, CV_32FC3);
    drawPlanarmap(mat, map);
    cv::imshow("t", mat);
    cv::waitKey();
}

static void removeContainingTriangle(Planarmap& map, std::shared_ptr<Triangle> triangle)
{
    std::vector<std::shared_ptr<Triangle>> toRemove;
    for (auto& t: map.getTriangles())
    {
        for (auto& v: triangle->vertices)
        {
            if (t->getPointIndex(v) != -1)
            {
                toRemove.push_back(t);
                break;
            }
        }
    }

    for (auto& t: toRemove)
    {
        map.removeTriangle(t);
    }
}

std::vector<Line> mapToLines(Planarmap& map)
{
    std::vector<Line> lines;
    for (auto& t: map.getTriangles())
    {
        for (int i = 0; i < 3; i++)
        {
            auto& v = t->vertices[i];
            auto& next = t->vertices[(i + 1) % 3];
            lines.emplace_back(v->location, next->location);
        }
    }
    return lines;
}
Planarmap triangulate(std::vector<Point> points)
{
    Planarmap map;
    std::vector<std::shared_ptr<Vertex>> containingTriangle {
            map.addVertex(Point(-1000, -1000)),
            map.addVertex(Point(1000, -1000)),
            map.addVertex(Point(300, 2000))
    };

    auto triangle = map.addTriangle(containingTriangle);
    TriangleTree tree(triangle);

    std::shuffle(points.begin(), points.end(), std::mt19937(std::random_device()()));
    for (auto& point: points)
    {
        addPointToTree(map, tree, point);
    }

#ifdef DRAW
    cv::Mat mat = cv::Mat(600, 600, CV_32FC3);
    drawTriangles(mat, map);
    cv::imshow("Before legalization", mat);
    cv::waitKey();
#endif

    legalizeTriangles(map);
    removeContainingTriangle(map, triangle);

    return map;
}
