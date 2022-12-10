#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdint>

#include <ranges>
#include <unordered_set>

bool isUnique(auto subrange)
{
	std::unordered_set<char> characterSet;
	for(char character : subrange)
	{
		if (characterSet.find(character) != characterSet.end())
			return false;
		
		characterSet.insert(character);
	}
	return true;
}

int main()
{
	std::ifstream fileStream{ "input.txt", std::ios::in };
	assert(fileStream.is_open() && "file stream failed to open");
	std::string line;
	std::getline(fileStream, line);

	const int intentifierSize = 14;
	for(int i = 0; i < line.size() - intentifierSize; ++i)
	{
		auto slice = std::ranges::subrange(line.begin() + i, line.begin() + i + intentifierSize);
		if(isUnique(slice))
		{
			std::cout << i + intentifierSize << "\n";
			break;
		}
	}
}
