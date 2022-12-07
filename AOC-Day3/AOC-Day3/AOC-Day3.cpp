#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <stdint.h>

#include <span>
#include <unordered_map>
#include <unordered_set>

int main()
{
	std::ifstream fileStream{ "input.txt", std::ios::in };
	assert(fileStream.is_open() && "file stream failed to open");
	
	uint64_t totalSum{};

	while (!fileStream.eof())
	{
		std::unordered_map<char, int> itemCountMap;
		for (int i = 0; i < 3; ++i)
		{
			std::string line;
			std::getline(fileStream, line);
			
			std::unordered_set<char> uniqueItems{ line.begin(), line.end() };
			for (char item : uniqueItems)
				itemCountMap[item]++;
		}
		
		char item = std::find_if(itemCountMap.begin(), itemCountMap.end(), [](std::pair<const char, int> inPair) { 
			return inPair.second == 3;
		})->first;

		totalSum += (std::isupper(item) ? item - 'A' + 26 : item - 'a') + 1;
	}

	std::cout << totalSum << "\n";
}