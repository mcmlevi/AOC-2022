#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <stdint.h>

#include <span>
#include <unordered_set>

int main()
{
	std::ifstream fileStream{ "input.txt", std::ios::in };
	assert(fileStream.is_open() && "file stream failed to open");
	
	uint64_t totalSum{};

	while (!fileStream.eof())
	{
		std::string line;
		std::getline(fileStream, line);
		std::unordered_set<char> itemsInFirstCompartement{line.begin(), line.begin() + line.size() / 2};

		for (const char item : std::span{ line.begin() + line.size() / 2, line.end() })
		{
			if (itemsInFirstCompartement.find(item) != itemsInFirstCompartement.end())
			{
				totalSum += (std::isupper(item) ? item - 'A' + 26 : item - 'a') + 1;
				break;
			}
		}
	}

	std::cout << totalSum << "\n";
}