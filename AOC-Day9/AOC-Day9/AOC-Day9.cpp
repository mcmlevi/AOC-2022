#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdint>

#include <unordered_set>
#include <algorithm>

struct Vec2
{
	int x = 0;
	int y = 0;
	
	Vec2& operator+=(const Vec2 inOther)
	{
		x += inOther.x;
		y += inOther.y;
		return *this;
	}

	bool operator==(const Vec2& inOther) const
	{
		return x == inOther.x && y == inOther.y;
	}
};

namespace std {
	template <> struct hash<Vec2>
	{
		size_t operator()(const Vec2& inPosition) const
		{
			return std::hash<int>()(inPosition.x) ^ std::hash<int>()(inPosition.y);
		}
	};
}

Vec2 GetDirection(char inDirection)
{
	if (inDirection == 'L')
		return { -1, 0 };
	else if (inDirection == 'R')
		return { 1, 0 };
	else if (inDirection == 'U')
		return { 0 , -1 };
	else
		return { 0 , 1 };
}

bool IsTouching(const Vec2& inA, const Vec2& inB) 
{
	int xOffSet = std::abs(inA.x - inB.x);
	int yOffset = std::abs(inA.y - inB.y);
	
	if (xOffSet == 1 && yOffset == 1)
		return true;
	
	if (xOffSet == 1 && yOffset == 0)
		return true;
	
	if (xOffSet == 0 && yOffset == 1)
		return true;

	if (xOffSet == 0 && yOffset == 0)
		return true;
 
	return false;
}

Vec2 GetNewTailPos(const Vec2& inHead, const Vec2& inTail)
{
	int xOffSet = inHead.x - inTail.x;
	xOffSet = std::clamp(xOffSet, -1, 1);

	int yOffset = inHead.y - inTail.y;
	yOffset = std::clamp(yOffset, -1, 1);

	return { inTail.x + xOffSet, inTail.y + yOffset };
}

void MoveHead(Vec2& inCurrentHeadPos, std::vector<Vec2>& inKnots, char inDirection, int inCount, std::unordered_set<Vec2>& inVisisitedPositions)
{
	Vec2 directionToMove = GetDirection(inDirection);
	
	for (int i = 0; i < inCount; ++i)
	{
		inCurrentHeadPos += directionToMove;
		Vec2 PositionToCompareTo = inCurrentHeadPos;
		
		for (Vec2& knotPosition : inKnots)
		{				
			if (IsTouching(PositionToCompareTo, knotPosition))
			{
				PositionToCompareTo = knotPosition;
				continue;
			}

			knotPosition = GetNewTailPos(PositionToCompareTo, knotPosition);
			PositionToCompareTo = knotPosition;
		}

		inVisisitedPositions.insert(inKnots.back());
	}
}

int main()
{
	std::ifstream fileStream{ "input.txt", std::ios::in };
	assert(fileStream.is_open() && "file stream failed to open");

	std::unordered_set<Vec2> visitedPositions;

	Vec2 currentHeadPosition = { 0,0 };
	
	std::vector<Vec2> knots;
	knots.resize(9);

	visitedPositions.insert(knots.back());

	while(!fileStream.eof())
	{
		char direction;
		int count;
		fileStream >> direction >> count;
		MoveHead(currentHeadPosition, knots, direction, count, visitedPositions);
	}

	std::cout << "Positions visited: " << visitedPositions.size() << "\n";
}
