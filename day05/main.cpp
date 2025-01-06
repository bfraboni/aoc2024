#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <bitset>
#include <algorithm>
#include "../common/timer.h"

// AoC Day 5: https://adventofcode.com/2024/day/5
// Part 1: Get rules and find valid updates. Sum up the middle page number from them.
// Part 2: Reorder invalid updates. Sum up the middle page number from them.

//! NOTE: this solution only works for 2 digits page numbers
inline bool check(const std::vector<int>& data, const std::bitset<100*100>& rules)
{
    // for each page in the data, check that the page after respect the rules
    for (int i = 0; i < data.size(); i++)
        for (int j = i+1; j < data.size(); j++)
            if (!rules[data[i] * 100 + data[j]])
                return false;
    return true;
}

int main(int argc, char * argv[])
{   
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::bitset<100 * 100> rules;
    std::vector<int> data;
    data.reserve(100);
    std::string line;
    size_t part1 = 0ul, part2 = 0ul;
    while(std::getline(file, line))
    {
        std::istringstream iss(line);
        if (line.find('|') != std::string::npos) // parse rules
        {
            int a, b; char c; iss >> a >> c >> b;
            rules[a * 100 + b] = true;
        }
        else if (!line.empty()) // parse updates
        {
            // parse and store the data in an array
            data.clear(); int a; char c; while(iss >> a) { data.push_back(a); iss >> c; };

            const auto& cdata = data;
            if (check(data, rules))
            {
                part1 += cdata[cdata.size()/2];
            }
            else
            {
                //! reorder update with the rules
                std::sort(data.begin(), data.end(), [&](const int& a, const int& b){ return !rules[a * 100 + b]; });
                part2 += cdata[cdata.size()/2];
            }
        }
    }

    const double t1 = t.micro().count(); t.reset();
    printf("Part 1: %zu\n", part1);
    printf("Part 2: %zu\n", part2);
    printf("Time: %f µs\n", t1);
    return 0;
}