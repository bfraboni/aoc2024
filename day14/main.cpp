#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include "../common/timer.h"

// AoC Day 14: https://adventofcode.com/2024/day/14
// Part 1:
// Part 2:

inline int mod(const int i, const int n) { return (i % n + n) % n; }

inline float var(const int array[], const int size)
{
    float mean = 0, var = 0;
    for(int i = 0; i < size; i++)
    {
        mean += array[i];
        var += array[i] * array[i];
    }
    return std::abs(var / size - mean * mean / (size * size));
}

struct Robot { int px, py, vx, vy; };

int main(int argc, char * argv[])
{   
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::string line;
    std::vector<Robot> robots;
    robots.reserve(1000);
    constexpr int width = 101, height = 103;
    constexpr int halfw = width/2;
    constexpr int halfh = height/2;
    size_t quad[4] = { 0ul };
    while(std::getline(file, line))
    {
        Robot r;
        if (sscanf(line.c_str(), "p=%d,%d v=%d,%d", &r.px, &r.py, &r.vx, &r.vy))
        {
            robots.push_back(r);
            int px100 = mod(r.px + 100 * r.vx, width);
            int py100 = mod(r.py + 100 * r.vy, height);
            quad[0] += bool(px100 < halfw && py100 < halfh);
            quad[1] += bool(px100 < halfw && py100 > halfh);
            quad[2] += bool(px100 > halfw && py100 > halfh);
            quad[3] += bool(px100 > halfw && py100 < halfh);
        }
    }
    const size_t part1 = quad[0] * quad[1] * quad[2] * quad[3];
    const double t1 = t.micro().count(); t.reset();

    int xtime = 0, ytime = 0;
    float vx = 0, vy = 0;
    for (int t = 0; t < 103; ++t)
    {
        int xi[101] = {0}, yi[103] = {0}; // histograms of positions
        for(const auto& r : robots)
        {
            const int pxt = mod(r.px + t * r.vx, width);  xi[pxt] += 1;
            const int pyt = mod(r.py + t * r.vy, height); yi[pyt] += 1;
        }

        // find the time with highest variance in X
        const float lvx = var(xi, 101);
        if (t < 101 && lvx > vx)
        {
            vx = lvx;
            xtime = t;
        }
        // find the time with highest variance in Y
        const float lvy = var(yi, 103);
        if (lvy > vy)
        {
            vy = lvy;
            ytime = t;
        }
    }
    const size_t part2 = (5253 * xtime + 5151 * ytime) % (width * height); // chinese remainder theorem
    const double t2 = t.micro().count(); t.reset();
    printf("Part 1: %zu, Time: %f µs\n", part1, t1);
    printf("Part 2: %zu, Time: %f µs\n", part2, t2);
    return 0;
}