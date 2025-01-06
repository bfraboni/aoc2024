#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include "../common/timer.h"

// AoC Day 6: https://adventofcode.com/2024/day/6
// Part 1: The guard '^' can only move forward and turn right when blocked '#'. 
//         Count all distinct positions of the guard before he leaves the map.
// Part 2: For each unique position except the initial one, add an obstacle and
//         test if the guard make a loop. Count the number of loops.

struct Vec2
{
    static constexpr int dx[4] = { 0, 1, 0, -1 }, dy[4] = { -1, 0, 1, 0 };
    int x, y, dir;
    inline Vec2() {}
    inline Vec2(int x, int y, int dir = 0) : x(x), y(y), dir(dir) {}
    inline Vec2 next() const { return Vec2(x+dx[dir], y+dy[dir], dir); }
    inline Vec2 turn() const { return Vec2(x, y, (dir+1)%4); }
    inline int index(const int w, const int h) const { return y * w + x; }
    inline int hash(const int w, const int h) const { return y * w * 4 + x * 4 + dir; }
    inline bool valid(const int w, const int h) const { return x >= 0 && x < w && y >= 0 && y < h; }
};

inline bool solve1(const Vec2& start, const std::vector<std::string>& data, std::unordered_set<int>& set)
{
    const int h = data.size(), w = data[0].size();
    Vec2 current = start;
    set.emplace(current.index(w, h));
    while(current.valid(w, h))
    {
        const Vec2 next = current.next();
        if (!next.valid(w, h))              break;
        else if (data[next.y][next.x] == '#')    current = current.turn();
        else                                current = next;
        auto res = set.emplace(current.index(w, h));
    }
    return true;
}

inline bool solve2(const Vec2& start, const int index, const std::vector<std::string>& data, std::unordered_set<int>& set)
{
    const int h = data.size(), w = data[0].size();
    Vec2 current = start;
    set.emplace(current.hash(w, h));
    while(current.valid(w, h))
    {
        const Vec2 next = current.next();
        if (!next.valid(w, h))                  break;
        else if (data[next.y][next.x] == '#' || next.index(w, h) == index) current = current.turn();
        else                                    current = next;
        auto res = set.emplace(current.hash(w, h));
        // if we already visited the position, this is a loop;
        if (!res.second) return false;
    }
    return true;
}

int main(int argc, char * argv[])
{   
    // read map data and find guard initial position
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::vector<std::string> data;
    std::string line;
    Vec2 guard;
    while(std::getline(file, line))
    {
        data.push_back(line);
        auto pos = line.find('^');
        if (pos != std::string::npos)
        {
            guard = Vec2(pos, data.size()-1, 0); // default points up = 0
            data.back()[pos] = '.';
        }
    }

    // map size
    const int h = data.size(), w = data[0].size();

    // Part 1
    const double t1 = t.micro().count(); t.reset();
    std::unordered_set<int> set1;
    solve1(guard, data, set1);
    const size_t part1 = set1.size();

    // Part2
    //! TOFIX: this is pretty slow..
    const double t2 = t.micro().count(); t.reset();
    size_t part2 = 0;
    std::unordered_set<int> set2;
    const int gid = guard.index(w, h);
    for (const int& index : set1)
    {
        if (gid == index) continue; // we do not test for the initial position
        set2.clear();
        part2 += !solve2(guard, index, data, set2);
    }
    const double t3 = t.micro().count();

    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu Time: %f µs\n", part1, t2);
    printf("Part 2: %zu Time: %f µs\n", part2, t3);
    return 0;
}