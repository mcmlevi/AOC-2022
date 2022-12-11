#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdint>

#include <vector>

struct Tree
{
	int height;
	bool visibleFromOutside;
};

using Forest = std::vector<std::vector<Tree>>;
using Position = std::pair<int, int>;

Forest GetForest(std::ifstream& inFileStream)
{
	Forest forest;
	while (!inFileStream.eof())
	{
		std::string treeLineString;
		std::getline(inFileStream, treeLineString);

		std::vector<Tree> treeLine;
		treeLine.reserve(treeLineString.size());

		for (char height : treeLineString)
			treeLine.push_back({ height - '0', false });

		forest.emplace_back(std::move(treeLine));
	}

	return forest;
}

auto GetNextIndex = [](Position CurrentPos, int moveX, int moveY) -> Position {
	auto [y, x] = CurrentPos;
	x += moveX;
	y += moveY;
	return std::make_pair(y, x);
};

void CalculateVisability(Forest& inForest)
{
	auto IterateForetInDirection = [](Forest& inForest, Position inStartPos, int moveX, int moveY)
	{
		auto [y, x] = inStartPos;

		Tree* currentTree = &inForest[y][x];
		currentTree->visibleFromOutside = true;

		Position currentPosition = std::make_pair(y, x);
		int maxHeight = currentTree->height;
		while (true)
		{
			auto [y, x] = GetNextIndex(currentPosition, moveX, moveY);

			// Only check till the max possible size
			if (y < 0 || x < 0 || inForest.size() == y || inForest[y].size() == x)
				break;

			Tree& nextTree = inForest[y][x];
			if (maxHeight < nextTree.height)
			{
				maxHeight = std::max(maxHeight, nextTree.height);
				nextTree.visibleFromOutside = true;
			}

			currentPosition = std::make_pair(y, x);
			currentTree = &inForest[y][x];
		}
	};

	// Check top
	for(int i = 0; i < inForest.front().size(); ++i)
		IterateForetInDirection(inForest, std::make_pair(0, i), 0, 1);

	// Check bottom
	for (int i = 0; i < inForest.back().size(); ++i)
		IterateForetInDirection(inForest, std::make_pair(static_cast<int>(inForest.size()) - 1, i), 0, -1);

	// Check left
	for (int i = 0; i < inForest.size(); ++i)
		IterateForetInDirection(inForest, std::make_pair(i, 0), 1, 0);

	// Check right
	for (int i = 0; i < inForest.size(); ++i)
		IterateForetInDirection(inForest, std::make_pair(i, static_cast<int>(inForest[i].size()) - 1), -1, 0);
}

size_t GetMostScenicScore(const Forest& inForest)
{
	auto GetViewDistanceInDirection = [](const Forest& inForest, Position inStartPos, int moveX, int moveY)
	{
		auto [y, x] = inStartPos;

		const Tree* currentTree = &inForest[y][x];

		Position currentPosition = std::make_pair(y, x);
		int maxHeight = currentTree->height;
		size_t viewDistance = 0;
		while (true)
		{
			auto [y, x] = GetNextIndex(currentPosition, moveX, moveY);

			if (y < 0 || x < 0 || inForest.size() == y || inForest[y].size() == x)
				break;

			++viewDistance;

			const Tree& nextTree = inForest[y][x];
			if (maxHeight <= nextTree.height)
			{
				break;
			}

			currentPosition = std::make_pair(y, x);
			currentTree = &inForest[y][x];
		}

		return viewDistance;
	};


	size_t maxScore = 0;
	for (int y = 0; y < inForest.size(); ++y)
	{
		for (int x = 0; x < inForest[y].size(); ++x)
		{
			size_t scenicScore = 1;
			const Position currentPos = std::make_pair(y, x);
			scenicScore *= GetViewDistanceInDirection(inForest, currentPos, 1, 0);
			scenicScore *= GetViewDistanceInDirection(inForest, currentPos, -1, 0);
			scenicScore *= GetViewDistanceInDirection(inForest, currentPos, 0, 1);
			scenicScore *= GetViewDistanceInDirection(inForest, currentPos, 0, -1);
			maxScore = std::max(maxScore, scenicScore);
		}
	}
	return maxScore;
}


void PrintForest(const Forest& inForest)
{
	for (int y = 0; y < inForest.size(); ++y)
	{
		for (int x = 0; x < inForest[y].size(); ++x)
		{
			std::cout << inForest[y][x].visibleFromOutside;
		}
		std::cout << "\n";
	}
}

int main()
{
	std::ifstream fileStream{ "input.txt", std::ios::in };
	assert(fileStream.is_open() && "file stream failed to open");

	Forest forest = GetForest(fileStream);
	CalculateVisability(forest);

	size_t treesVisibleFromOutside = 0;
	for (const auto& treeLine : forest)
	{
		for (const auto& tree : treeLine)
		{
			treesVisibleFromOutside += tree.visibleFromOutside;
		}
	}

	std::cout << treesVisibleFromOutside << "\n";
	std::cout << "Highest Scenic score: " << GetMostScenicScore(forest) << "\n";

	PrintForest(forest);
}
