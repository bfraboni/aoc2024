#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include "../common/timer.h"

// AoC Day 13
// Part 1:
// Part 2:

int main(int argc, char * argv[])
{   
    // read all data at once
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::string line;
    std::vector<std::string> map;
    int ax, ay, bx, by, x, y;
    size_t part1 = 0ul, part2 = 0ul;
    while(std::getline(file, line))
    {
        if (line.empty()) continue;
        else if (sscanf(line.c_str(), "Button A: X+%d, Y+%d", &ax, &ay))
        {
            std::getline(file, line); sscanf(line.c_str(), "Button B: X+%d, Y+%d", &bx, &by);
            std::getline(file, line); sscanf(line.c_str(), "Prize: X=%d, Y=%d", &x, &y);

            // we parsed a full problem, now just solve it
            auto solve = [](int64_t x, int64_t y, int64_t ax, int64_t ay, int64_t bx, int64_t by, int64_t det) -> int64_t
            {
                const int64_t A = (x * by - y * bx) / det;
                const int64_t B = (y * ax - x * ay) / det;
                const bool valid = (x == A * ax + B * bx) && (y == A * ay + B * by);
                return valid * (3 * A + B);
            };

            constexpr int64_t offset = 10000000000000;
            const int64_t det = ax * by - ay * bx;
            part1 += solve(x, y, ax, ay, bx, by, det);
            part2 += solve(x+offset, y+offset, ax, ay, bx, by, det);
        }
    }

    const double t1 = t.micro().count(); t.reset();
    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu\n", part1);
    printf("Part 2: %zu\n", part2);
    return 0;
}