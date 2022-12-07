#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdint>

#include <cstdio>

int main()
{
	std::ifstream fileStream{ "input.txt", std::ios::in };
	assert(fileStream.is_open() && "file stream failed to open");

	using Range = std::pair<int, int>;

	uint64_t numberOfContainedRanges{};
	while (!fileStream.eof())
	{
		std::string line;
		std::getline(fileStream, line);

		Range elfOneRange;
		Range elfTwoRange;
		sscanf_s(line.c_str(), "%i-%i,%i-%i", &elfOneRange.first, &elfOneRange.second, &elfTwoRange.first, &elfTwoRange.second);

		if (elfTwoRange.first < elfOneRange.first)
			std::swap(elfOneRange, elfTwoRange);
		else if (elfTwoRange.first == elfOneRange.first && elfOneRange.second < elfTwoRange.second)
			std::swap(elfOneRange, elfTwoRange);

		if (elfTwoRange.first >= elfOneRange.first && elfTwoRange.second <= elfOneRange.second)
			++numberOfContainedRanges;
	}

	std::cout << numberOfContainedRanges << "\n";
}