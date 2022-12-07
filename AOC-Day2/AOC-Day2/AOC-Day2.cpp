
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <stdint.h>

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

		char opponentChoice = line[0] - 'A' + 1;
		char myChoice = line[2] - 'X' + 1;

		totalScore += myChoice;

		int gameResult = opponentChoice - myChoice;
		
		if (gameResult == -1 || gameResult == 2)
			totalScore += 6;
		else if (gameResult == 0)
			totalScore += 3;
	}

	std::cout << totalScore << "\n";
}