#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "../common/timer.h"

// AoC Day 2 
// Part 1: Count sets that are all decreasing or all increasing with distance in range [1, 3].
// Part 2: Extend to sets where the conditions are met while removing at most one number.

enum Order : bool { Increasing = 0, Decreasing = 1 };
template<Order order>
bool check(const std::vector<int>& data)
{
    int prev = data[0];
    for (int i = 1; i < data.size(); ++i)
    {
        const int next = data[i];
        const int diff = next - prev;
        const bool valid = order == Increasing ? (diff > 0 && diff < 4) : (diff >-4 && diff < 0);
        if (valid)  prev = next;
        else        return false;
    }
    return true;
};

int main(int argc, char * argv[])
{
    Timer t;
	std::fstream file("input.txt", std::ios_base::in);
    std::string line;
    size_t part1 = 0ul, part2 = 0ul;
    std::vector<int> data, copy;
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
            // slightly brute-force solution
            const int size = data.size();
            for (int i = 0; i < size; ++i)
            {
                copy.clear();
                for (int j = 0; j < size; j += j == i ? 2 : 1)
                    copy.push_back(data[j]);

                if (check<Increasing>(copy) || check<Decreasing>(copy)) // safe for part 2
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