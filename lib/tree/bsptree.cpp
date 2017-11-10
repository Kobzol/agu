#include "bsptree.h"
#include "../algo.h"
#include "../draw.h"

static BspTree* buildTree(Line line, std::vector<Line> lines)
{
    auto tree = new BspTree(line);
	if (lines.empty()) return tree;

	// 0 - left, 1 - right
	std::vector<Line> halves[2];
	for (int i = 0; i < lines.size(); i++)
	{
		auto candidate = lines[i];
		int tests[2] = {
			testPointCCW(candidate.first, line.first, line.second),
			testPointCCW(candidate.second, line.first, line.second)
		};

		cv::Mat test = cv::Mat(600, 600, CV_32FC3);

		if (tests[0] >= 0 && tests[1] >= 0)
		{
			halves[0].push_back(candidate);
			drawLines(test, halves[0]);
		}
		else if (tests[0] <= 0 && tests[1] <= 0)
		{
			halves[1].push_back(candidate);
			drawLines(test, halves[1]);
		}
		else // split
		{
 			auto intersection = bisectorIntersection(line, candidate);
			lines.emplace_back(candidate.first, intersection);
			lines.emplace_back(intersection, candidate.second);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if (!halves[i].empty())
		{
			auto root = halves[i].back();
			halves[i].resize(halves[i].size() - 1);
			tree->nodes[i] = buildTree(root, halves[i]);
		}
	}

	return tree;
}

std::unique_ptr<BspTree> BspTree::buildTree(std::vector<Line> lines)
{
	auto line = lines.back();
	lines.resize(lines.size() - 1);

    return std::unique_ptr<BspTree>(::buildTree(line, lines));
}

BspTree::BspTree(Line line): line(line)
{

}

BspTree::~BspTree()
{
    for (auto& node : this->nodes)
    {
        delete node;
    }
}

bool BspTree::isLeaf() const
{
	return this->nodes[0] == nullptr && this->nodes[1] == nullptr;
}

void BspTree::paint(Point view, std::function<void(const BspTree& tree)> callback) const
{
	if (this->isLeaf())
	{
		callback(*this);
		return;
	}

	// left
	int order[2] = { 1, 0 };

	// right
	if (testPointCCW(view, this->line.first, this->line.second) < 0)
	{
		std::swap(order[0], order[1]);
	}

	if (this->nodes[order[0]] != nullptr)
	{
		this->nodes[order[0]]->paint(view, callback);
	}
	callback(*this);
	if (this->nodes[order[1]] != nullptr)
	{
		this->nodes[order[1]]->paint(view, callback);
	}
}
