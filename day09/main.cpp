#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "../common/timer.h"

// AoC Day 9: https://adventofcode.com/2024/day/9
// Part 1: Compress data, take the leftmost blocks and push them back in the front empty space.
// Part 2: Avoid data fragmentation by copying rightmost memory block to the first leftmost empty space of same size.

inline size_t solve1(std::vector<int>& data)
{
    size_t res = 0ul;
    for (size_t lid = 0, rid = data.size()-1; lid <= rid; lid++)
    {
        if (data[lid] < 0) // there is an empty space to fill
        {
            while(data[rid] < 0 && rid > lid) rid--; // find first non empty cell
            std::swap(data[lid], data[rid]); // swap values
        }
        if (data[lid] >= 0) // if the space is filled
            res += data[lid] * lid;
        else
            break;
    }
    return res;
}

int main(int argc, char * argv[])
{   
    // read all data at once
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::vector<int> data;
    char c; int count = 0;
    struct Block { int start, end, size; };
    std::vector<Block> full, empty; 
    while(file >> c)
    {
        const bool nonempty = count%2 == 0;
        const int n = c-'0';
        const int id = nonempty ? count/2 : -n; // encode empty space with negative values
        const Block b = {(int)data.size(), (int)data.size()+n, n};
        if (nonempty)   full.emplace_back(b);
        else            empty.emplace_back(b);
        data.insert(data.end(), n, id);
        count++;
    }
    const double t1 = t.micro().count(); t.reset();

    // part 1
    std::vector<int> copy = data;
    const size_t part1 = solve1(copy);
    const double t2 = t.micro().count(); t.reset();

    // part 2
    // for each data block in reverse order
    std::reverse(full.begin(), full.end());
    for (const auto& f : full)
    {
        // find any empty block that'd hold that data
        for (auto& e : empty)
        {
            if (e.size < f.size || e.end > f.start) continue;

            for(int i = 0; i < f.size; ++i)
                std::swap(data[f.start+i], data[e.start+i]);

            e.start += f.size;
            e.size = e.end - e.start;
            break;
        }
    }

    size_t id = 0, part2 = 0ul;
    for(const auto& i : data)
    {
        if (i >= 0) part2 += i * id;
        id++;
    }

    const double t3 = t.micro().count(); t.reset();
    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu, time: %f µs\n", part1, t2);
    printf("Part 2: %zu, time: %f µs\n", part2, t3);
    return 0;
}