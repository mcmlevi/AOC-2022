#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdint>
#include <vector>
#include <array>

using Screen = std::array<std::array<char,40>, 6>;

bool CheckCycle(std::vector<int>::iterator& inIterator, const std::vector<int>& inCycles, size_t inCycle, int inRegisterX, size_t& inSumOfSignalsStrenghts)
{
	if (inCycle == *inIterator)
	{
		inSumOfSignalsStrenghts += inRegisterX * inCycle;
		++inIterator;
	}

	return inIterator != inCycles.end();
}

bool ShouldDrawPixel(int inA, int inB)
{
	int offset = std::abs(inA - inB);
	return offset <= 1;
}


void UpdateScreen(Screen& inScreen, size_t inCycle, size_t screenCycleCount, int inRegX)
{
	inCycle %= screenCycleCount;
	--inCycle;
	
	int row = inCycle / inScreen[0].size();
	int column = inCycle - row * inScreen[0].size();

	if (ShouldDrawPixel(inRegX, column))
		inScreen[row][column] = '#';
}

void ClearScreen(Screen& inScreen)
{
	for (auto& row : inScreen)
	{
		for (char& pixel : row)
		{
			pixel = '.';
		}
	}
}

void PrintScreen(const Screen& inScreen)
{
	for (const auto& row : inScreen)
	{
		for (const char& pixel : row)
		{
			std::cout << pixel;
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void UpdateCycle(size_t& inCycle, Screen& inScreen, size_t inRegisterX, size_t inDisplayCycleCount)
{
	++inCycle;
	UpdateScreen(inScreen, inCycle, inDisplayCycleCount, inRegisterX);
	
	if (inCycle == inDisplayCycleCount)
	{
		PrintScreen(inScreen);
		ClearScreen(inScreen);
	}
}

int main()
{
	std::ifstream fileStream{ "input.txt", std::ios::in };
	assert(fileStream.is_open() && "file stream failed to open");

	std::vector<int> cyclesToCheck{40,80,120,160,200,240};
	auto currentCycle = cyclesToCheck.begin();
	
	const int screenCycleCount = cyclesToCheck.back();

	Screen screen;
	ClearScreen(screen);

	size_t cycle = 0;
	size_t registerX = 1;
	size_t sumOfSignalStrenghts = 0;

	while(!fileStream.eof())
	{
		UpdateCycle(cycle, screen, registerX, screenCycleCount);

		std::string instruction;
		fileStream >> instruction;

		if (!CheckCycle(currentCycle, cyclesToCheck, cycle, registerX, sumOfSignalStrenghts))
			break;

		if(instruction == "noop")
			continue;

		if(instruction == "addx")
		{
			int valueToAdd;
			fileStream >> valueToAdd;

			UpdateCycle(cycle, screen, registerX, screenCycleCount);

			if (!CheckCycle(currentCycle, cyclesToCheck, cycle, registerX, sumOfSignalStrenghts))
				break;

			registerX += valueToAdd;
		}
	}

	std::cout << sumOfSignalStrenghts;
}