
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <stdint.h>
#include <array>

const std::array<char, 3> looseLookupTable{2, 0, 1};
const std::array<char, 3> winLookupTable{1, 2, 0};

int main()
{
	std::ifstream fileStream{ "input.txt", std::ios::in };
	assert(fileStream.is_open() && "file stream failed to open");

	uint64_t totalScore{};
	while (!fileStream.eof())
	{
		std::string line;
		std::getline(fileStream, line);

		assert(line.size() == 3, "unexpected format");

		char opponentChoice = line[0] - 'A';
		char desiredGameResult = line[2] - 'X';

		if (desiredGameResult == 0)
			totalScore += looseLookupTable[opponentChoice] + 1;
		else if (desiredGameResult == 1)
			totalScore += opponentChoice + 1 + 3;
		else
			totalScore += winLookupTable[opponentChoice] + 1 + 6;
	}

	std::cout << totalScore << "\n";
}