#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include "../common/timer.h"

// AoC Day 12
// Part 1: Walls and equivalence class.. Union Find
// Part 2:

struct Vec2
{
    int x, y;
    Vec2() {}
    Vec2(int x, int y) : x(x), y(y) {}
    Vec2(int id, int w, int h) : x(id % w), y(id / w) {}
    int index(const int w, const int h) const { return y * w + x; }
    bool valid(const int w, const int h) const { return x >= 0 && x < w && y >= 0 && y < h; }
    void print_pos() const { std::cout << "(" << x << " " << y << ")"; }
    Vec2 operator+(const Vec2& other) const { return Vec2(x+other.x, y+other.y); }
    Vec2 operator-(const Vec2& other) const { return Vec2(x-other.x, y-other.y); }
};

inline int find(const int id, std::vector<int>& data)
{
    if (id == data[id]) return id;
    return data[id] = find(data[id], data); // path compression
}

inline std::pair<size_t, size_t> solve(const std::vector<std::string>& map)
{
    const size_t h = map.size(), w = map[0].size();
    std::vector<int> fences(w * h * 4, 1), regions(w * h); // could use a bit compressed version..
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
        const Vec2 neighbor = pos + Vec2(dx[fence], dy[fence]);

        // on border wall, simply continue
        if (!neighbor.valid(w, h)) continue;

        // test if they are in the same class
        if (map[pos.y][pos.x] == map[neighbor.y][neighbor.x])
        {
            // put down the fence
            fences[i] = 0;

            // make sure their roots are the same
            const int r1 = find(pos.index(w, h), regions);
            const int r2 = find(neighbor.index(w, h), regions);
            if (r1 != r2) regions[r2] = r1;
        }
    }

    // count the number of region, of cells per region, of fences per region, and of corners per region
    struct Region { size_t area = 0, perimeter = 0, corners = 0; };
    std::unordered_map<int, Region> maps;
    for (int i = 0; i < w * h; ++i)
    {
        const int region = find(i, regions);
        Region& reg = maps[region];
        // area
        reg.area++;
        // perimeter
        for (int j = 0; j < 4; ++j) reg.perimeter += bool(fences[i*4+j]);
        //  corners
        //  |4| |1|
        //  +-+-+-+
        //    |A| 
        //  +-+-+-+
        //  |3| |2|
        const Vec2 pos(i, w, h);
        constexpr int next[4] = { 1, 2, 3, 0 }
                    , dx[4] = { 1, 1, -1, -1 }
                    , dy[4] = { -1, 1, 1, -1 }
                    , np[4] = { 2, 3, 0, 1 }
                    , nn[4] = { 3, 0, 1, 2 };

        for (int j = 0; j < 4; ++j)
        {
            const bool fa = fences[i*4+j], fb = fences[i*4+next[j]];
            bool iscorner = fa && fb;
            if (!iscorner && (!fa && !fb))
            {
                const Vec2 neighbor = Vec2(i, w, h) + Vec2(dx[j], dy[j]);
                if (neighbor.valid(w, h)) // if diagonal neighbor exists
                {
                    const int n = neighbor.index(w, h);
                    iscorner = (bool(fences[n*4+np[j]]) && bool(fences[n*4+nn[j]]));
                }
            }

            reg.corners += iscorner;
        }
    }

    size_t part1 = 0ul, part2 = 0ul;
    for (const auto& p : maps)
    {
        part1 += p.second.area * p.second.perimeter;
        part2 += p.second.area * p.second.corners;
        const Vec2 pos(p.first, w, h);
        const char c = map[pos.y][pos.x]; 
        // printf("region %d type %c area %zu perimeter %zu corners %zu price %zu discount %zu\n"
        //     , p.first
        //     , c
        //     , p.second.area
        //     , p.second.perimeter
        //     , p.second.corners
        //     , p.second.area * p.second.perimeter
        //     , p.second.area * p.second.corners
        // );
    }

    return {part1, part2};
}


int main(int argc, char * argv[])
{   
    // read all data at once
    Timer t;
#if 1
    std::fstream file("input.txt", std::ios_base::in);
#else
    std::string filedata = "AAAA\nBBCD\nBBCC\nEEEC\n";
    std::stringstream file(filedata);
#endif
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