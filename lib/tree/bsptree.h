#pragma once

#include <memory>
#include <functional>

#include "../common.h"

class BspTree
{
public:
    static std::unique_ptr<BspTree> buildTree(std::vector<Line> lines);

	BspTree(Line line);
    ~BspTree();

	bool isLeaf() const;

	void paint(Point view, std::function<void(const BspTree& tree)> callback) const;

    BspTree* nodes[2]{};
	Line line;
};
