#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "../common/timer.h"

// AoC Day 2: https://adventofcode.com/2024/day/2
// Part 1: Count sets that are all decreasing or all increasing with distance in range [1, 3].
// Part 2: Extend to sets where the conditions are met while removing at most one number.

enum Order : bool { Increasing = 0, Decreasing = 1 };
template<Order order>
inline bool check(const std::vector<int8_t>& data, int skip = -1)
{
    const int start = skip == 0 ? 1 : 0;
    int8_t prev = data[start];
    for (int i = start+1; i < data.size(); ++i)
    {
        if (i == skip) continue;
        const int8_t next = data[i];
        const int8_t diff = next - prev;
        const bool valid = order == Increasing ? (diff > 0 && diff < 4) : (diff >-4 && diff < 0);
        if (!valid) return false;
        prev = next;
    }
    return true;
};

int main(int argc, char * argv[])
{
    Timer t;
	std::fstream file("input.txt", std::ios_base::in);
    std::string line;
    size_t part1 = 0ul, part2 = 0ul;
    std::vector<int8_t> data;
    while(std::getline(file, line))
    {
        data.clear();
        std::istringstream parser(line);
        int a; while(parser >> a) data.push_back(a);

        if (check<Increasing>(data) || check<Decreasing>(data)) // safe for part1
        {
            part1++;
            part2++;
        }
        else
        {
            const int size = data.size();
            for (int i = 0; i < size; ++i)
            {
                if (check<Increasing>(data, i) || check<Decreasing>(data, i)) // safe for part 2
                {
                    part2++;
                    break;
                }
            }
        }
    }

    // print results
    const double t1 = t.micro().count();
    printf("Part 1: %zu\nPart 2: %zu\nTime:   %f µs\n", part1, part2, t1);

    return 0;
}