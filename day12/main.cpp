#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <bitset>
#include <unordered_map>
#include "../common/timer.h"

// AoC Day 12: https://adventofcode.com/2024/day/12
// Part 1: Walls and equivalence class.. Union Find; compute fences cost as area * perimeter and sum for each region. 
// Part 2: Compute fences cost as area * corners and sum for each region.

struct Vec2
{
    int x, y;
    inline Vec2() {}
    inline Vec2(int x, int y) : x(x), y(y) {}
    inline Vec2(int id, int w, int h) : x(id % w), y(id / w) {}
    inline int index(const int w, const int h) const { return y * w + x; }
    inline bool valid(const int w, const int h) const { return x >= 0 && x < w && y >= 0 && y < h; }
    inline Vec2 operator+(const Vec2& other) const { return Vec2(x+other.x, y+other.y); }
    inline Vec2 operator-(const Vec2& other) const { return Vec2(x-other.x, y-other.y); }
};

struct Region { size_t area = 0, perimeter = 0, corners = 0; };

inline int find(const int id, std::vector<int>& data)
{
    if (id == data[id]) return id;
    return data[id] = find(data[id], data); // path compression
}

inline std::pair<size_t, size_t> solve(const std::vector<std::string>& map)
{
    const size_t h = map.size(), w = map[0].size();
    std::vector<int> regions(w * h);
    std::vector<bool> fences(w * h * 4, true); // could use a bit compressed version..
    for (int i = 0; i < w * h; ++i) regions[i] = i;

    for (int i = 0; i < w * h * 4; ++i)
    {
        const int id = i / 4;
        const Vec2 pos(id % w, id / w);
        //    (0)
        //    +-+
        //(3) |i| (1)
        //    +-+
        //    (2)
        const int fence = i % 4;
        static constexpr int dx[4] = { 0, 1, 0, -1 }, dy[4] = { -1, 0, 1, 0 };
        const Vec2 neigh = pos + Vec2(dx[fence], dy[fence]);
        const int nid = id + dx[fence] + w * dy[fence];

        // on border wall, simply continue
        if (!neigh.valid(w, h)) continue;

        // test if they are in the same class
        else if (map[pos.y][pos.x] == map[neigh.y][neigh.x])
        {
            // put down the fence
            fences[i] = false;

            // root union are the same
            const int r1 = find(id, regions);
            const int r2 = find(nid, regions);
            if (r1 != r2) regions[r2] = r1;
        }
    }

    // count the number of region, of cells per region, of fences per region, and of corners per region
    std::unordered_map<int, Region> maps;
    for (int i = 0; i < w * h; ++i)
    {
        Region& reg = maps[find(i, regions)];
        // area
        reg.area++;
        //  corners
        //  |4| |1|
        //  +-+-+-+
        //    |A| 
        //  +-+-+-+
        //  |3| |2|
        const Vec2 pos(i, w, h);
        static constexpr int dx[4] = { 1, 1, -1, -1 }, dy[4] = { -1, 1, 1, -1 };
        for (int j = 0; j < 4; ++j)
        {
            const bool fa = fences[i*4+j], fb = fences[i*4+(j+1)%4], nofences = !fa && !fb;
            bool iscorner = fa && fb;
            if (nofences)
            {
                const Vec2 neighbor(pos.x+dx[j], pos.y+dy[j]);
                if (neighbor.valid(w, h)) // if diagonal neighbor exists
                {
                    const int nid = i + dx[j] + w * dy[j]; // neighbor.index(w, h);
                    iscorner = (fences[nid*4+(j+2)%4] && fences[nid*4+(j+3)%4]);
                }
            }
            reg.perimeter += fa;
            reg.corners += iscorner;
        }
    }

    size_t part1 = 0ul, part2 = 0ul;
    for (const auto& p : maps)
    {
        part1 += p.second.area * p.second.perimeter;
        part2 += p.second.area * p.second.corners;
    }

    return {part1, part2};
}


int main(int argc, char * argv[])
{   
    // read all data at once
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::string line;
    std::vector<std::string> map;
    while(std::getline(file, line)) map.push_back(line);

    const double t1 = t.micro().count(); t.reset();
    const std::pair<size_t, size_t> res = solve(map);
    const double t2 = t.micro().count(); t.reset();

    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu\n", res.first);
    printf("Part 2: %zu\n", res.second);
    printf("Time: %f µs\n", t2);
    return 0;
}