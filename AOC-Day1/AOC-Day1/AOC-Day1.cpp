#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <vector>
#include <stdint.h>
#include <algorithm>
#include <format>
#include <numeric>

int main()
{
    std::ifstream fileStream{"input.txt", std::ios::in };
    assert(fileStream.is_open() && "file stream failed to open");

    std::vector<std::uint64_t> caloriesPerElfList;

    std::uint64_t calloriesOnElf{};
    while(!fileStream.eof())
    {
        std::string line;
        std::getline(fileStream, line);

        if(line.empty())
        {
            caloriesPerElfList.push_back(calloriesOnElf);
            calloriesOnElf = 0;
            continue;
        }

        calloriesOnElf += std::stoull(line);
    }

    std::ranges::sort(caloriesPerElfList, std::ranges::greater());

    const size_t topElfCount = 3;
    std::uint64_t calloriesOnTopNElfs = std::accumulate(caloriesPerElfList.begin(), caloriesPerElfList.begin() + topElfCount, 0);

    std::cout << calloriesOnTopNElfs << "\n";
}