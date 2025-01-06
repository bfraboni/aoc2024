#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "../common/timer.h"

// AoC Day 10
// Part 1: Find the trailheads connected to a trailend (start with 0 and end with 9 with a constant slope of 1)
//         and sum of the scores (number of trailend reachable) of all trailheads on your topographic map.
// Part 2: Count and sum the ratings of each trailhead, i.e. the number of distinct trails that begins at that trailhead. 

struct Vec2
{
    int x, y;
    inline Vec2() {}
    inline Vec2(int x, int y, int dir = 0) : x(x), y(y) {}
    inline int index(const int w, const int h) const { return y * w + x; }
    inline bool valid(const int w, const int h) const { return x >= 0 && x < w && y >= 0 && y < h; }
    inline Vec2 operator+(const Vec2& other) const { return Vec2(x+other.x, y+other.y); }
    inline Vec2 operator-(const Vec2& other) const { return Vec2(x-other.x, y-other.y); }
};

struct Trailhead { size_t score; size_t ratings; };

inline void test( Trailhead& th, const Vec2& pos, const std::vector<std::string>& map
                , const int width, const int height, std::unordered_set<int>& ends)
{
    const bool leaf = map[pos.y][pos.x] == '9';
    if(!leaf)
    {
        auto traverse = [&](int dx, int dy)
        {
            const Vec2 next(pos.x + dx, pos.y + dy);
            if (next.valid(width, height) && map[next.y][next.x] - map[pos.y][pos.x] == 1)
                test(th, next, map, width, height, ends);
        };
        traverse(0, 1);
        traverse(1, 0);
        traverse(0, -1);
        traverse(-1, 0);
    }
    else
    {
        // don't double count the same position
        auto it = ends.find(pos.index(width, height));
        if (it == ends.end())
        {
            ends.insert(pos.index(width, height));
            th.score++;
        }
        th.ratings++;
    }
}

inline Trailhead test(const Vec2& start, const std::vector<std::string>& map, std::unordered_set<int>& ends)
{
    ends.clear();
    const int h = map.size(), w = map[0].size();
    Trailhead th = { 0ul, 0ul };
    test(th, start, map, h, w, ends);
    return th;
}

int main(int argc, char * argv[])
{
    using Array = std::vector<Vec2>;

    // read all data at once
    Timer t;
#if 1
    std::fstream file("input.txt", std::ios_base::in);
#else
    std::string filedata = "0123\n1234\n8765\n9876";
    std::stringstream file(filedata);
#endif
    std::string line;
    std::vector<Vec2> origins;
    std::vector<std::string> map;
    size_t lines = 0, length = 0;
    while(std::getline(file, line))
    {
        ++lines;
        map.push_back(line);
        size_t a = 0;
        while (true)
        {
            const size_t b = line.find('0', a);
            if (b == std::string::npos) break;
            origins.push_back(Vec2(b, lines-1));
            a = b+1;
        }
    }

    const double t1 = t.micro().count(); t.reset();
    // 
    size_t part1 = 0ul, part2 = 0ul;
    std::unordered_set<int> ends;
    for (const auto& start : origins)
    {
        Trailhead th = test(start, map, ends);
        part1 += th.score;
        part2 += th.ratings;
    }

    const double t2 = t.micro().count(); t.reset();

    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu\n", part1);
    printf("Part 2: %zu\n", part2);
    printf("Time: %f µs\n", t2);
    return 0;
}