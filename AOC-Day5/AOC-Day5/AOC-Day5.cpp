#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdint>

#include <ranges>
#include <vector>
#include <deque>

using containerList = std::vector<std::deque<char>>;

containerList GetContainers(std::ifstream& inFileStream)
{
	std::string line;
	std::getline(inFileStream, line);
	containerList containers;

	while(!line.empty())
	{
		auto stride = [](int n) {
			return[s = -1, n](auto const&) mutable { s = (s + 1) % n; return !s; };
		};

		auto containerRange = std::views::iota(0, static_cast<int>(line.size() - 1)) | std::views::filter(stride(4));
		
		containers.resize(std::distance(std::begin(containerRange), std::end(containerRange)));

		int containerIndex = 0;
		for (auto container : containerRange)
		{
			// The first container starts at index 0 so the item is 1 index away, any other cargo item is 2 items away as the index is on the space.
			char cargo = line[container + (containerIndex == 0 ? 1 : 2)];
			bool isValidContainer = cargo != ' ' && !std::isdigit(cargo);
			
			if (isValidContainer)
				containers[containerIndex].push_front(cargo);

			++containerIndex;
		}

		std::getline(inFileStream, line);
	}

	return containers;
}

struct Instruction
{
	int count;
	int source;
	int destination;
};

Instruction GetInstruction(const std::string& inLine)
{
	Instruction instruction;
	sscanf_s(inLine.c_str(), "move %i from %i to %i", &instruction.count, &instruction.source, &instruction.destination);
	
	// The index in the instruction is one based but we need it to be zero based so we translate it here.
	--instruction.source;
	--instruction.destination;
	
	return instruction;
}

int main()
{
	std::ifstream fileStream{ "input.txt", std::ios::in };
	assert(fileStream.is_open() && "file stream failed to open");
	
	containerList containers = GetContainers(fileStream);

	while (!fileStream.eof())
	{
		std::string line;
		std::getline(fileStream, line);

		Instruction instruction = GetInstruction(line);

		std::deque<char> stackToMove;
		for(int i = 0; i < instruction.count; ++i)
		{
			auto& sourceContainer = containers[instruction.source];
			stackToMove.push_front(sourceContainer.back());
			sourceContainer.pop_back();
		}

		for(char chargoItem: stackToMove)
			containers[instruction.destination].push_back(chargoItem);
	}

	std::string topCargoList;
	for(const auto& container: containers)
		topCargoList += container.back();

	std::cout << topCargoList;
}