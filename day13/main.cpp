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
        // std::istringstream iss(line);
        // if (line.find('A') != std::string::npos)
        if (sscanf(line.c_str(), "Button A: X+%d, Y+%d", &ax, &ay))
        {
            // int params = sscanf(line.c_str(), "Button A: X+%d, Y+%d", &ax, &ay);
            // iss.seekg(12ul); iss >> ax;
            // const size_t pos = iss.tellg();
            // iss.seekg(pos+4ul); iss >> ay;
            std::getline(file, line);
            sscanf(line.c_str(), "Button B: X+%d, Y+%d", &bx, &by);
            std::getline(file, line);
            sscanf(line.c_str(), "Prize: X=%d, Y=%d", &x, &y);
        // }
        // // else if (line.find('B') != std::string::npos)
        // else if (sscanf(line.c_str(), "Button B: X+%d, Y+%d", &bx, &by))
        // {
        //     // int params = sscanf(line.c_str(), "Button B: X+%d, Y+%d", &bx, &by);
        //     // iss.seekg(12ul); iss >> bx;
        //     // const size_t pos = iss.tellg();
        //     // iss.seekg(pos+4ul); iss >> by;
        // }
        // // else if (line.find('P') != std::string::npos)
        // else if (sscanf(line.c_str(), "Prize: X=%d, Y=%d", &x, &y))
        // {
            // int params = sscanf(line.c_str(), "Prize: X=%d, Y=%d", &x, &y);
            // iss.seekg(9ul); iss >> x;
            // const size_t pos = iss.tellg();
            // iss.seekg(pos+4ul); iss >> y;

            // we parsed a full problem, now just solve it
            const int64_t det = (ax * by - ay * bx);
            const int64_t A1 = (x * by - y * bx) / det;
            const int64_t B1 = (y * ax - x * ay) / det;
            const bool valid1 = (x == A1 * ax + B1 * bx) && (y == A1 * ay + B1 * by);
            constexpr int64_t offset = 10000000000000;
            const int64_t A2 = ((x+offset) * by - (y+offset) * bx) / det;
            const int64_t B2 = ((y+offset) * ax - (x+offset) * ay) / det;
            const bool valid2 = (x+offset == A2 * ax + B2 * bx) && (y+offset == A2 * ay + B2 * by);
            part1 += valid1 * (3 * A1 + B1);
            part2 += valid2 * (3 * A2 + B2);
        }
    }

    const double t1 = t.micro().count(); t.reset();

    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu\n", part1);
    printf("Part 2: %zu\n", part2);
    // printf("Time: %f µs\n", t2);
    return 0;
}