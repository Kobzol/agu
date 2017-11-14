#include "triangletree.h"

#include <utility>

#include "../algo.h"
#include "../draw.h"

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

		for (auto& child : this->children)
		{
			cv::Mat mat = cv::Mat::zeros(600, 600, CV_32FC3);
			drawTriangle(mat, *child->triangle);
			drawTriangle(mat, *this->triangle, cv::Scalar(0.0f, 0.0f, 1.0f));
			drawPoints(mat, { point });

			cv::imshow("Test", mat);
			cv::waitKey(0);
		}

        throw "wrong tree structure";
    }

    return nullptr;
}
