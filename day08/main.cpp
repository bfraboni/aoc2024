#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "../common/timer.h"

// AoC Day 8: https://adventofcode.com/2024/day/8
// Part 1: Count the number of antinodes on the map: on each sides of a pair of antennas of the same type.
// Part 1: Count the number of antinodes on the map: on the line formed by a pair of antennas of the same type.

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

using Array = std::vector<Vec2>;

int main(int argc, char * argv[])
{   

    // read all data at once
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::string line;
    // gather antennas position in a hashmap
    std::unordered_map<char, Array> map;
    size_t lines = 0, length = 0;
    while(std::getline(file, line))
    {
        ++lines;                            // update number of lines
        if (!length) length = line.size();  // get line length once
        std::istringstream iss(line);
        char c;
        while(iss >> c)
        {
            if (c == '.') continue;
            auto it = map.find(c);
            if (it == map.end()) it = map.emplace(c, Array()).first;
            it->second.push_back(Vec2(size_t(iss.tellg())-1, lines-1));
        }
    }
    const double t1 = t.micro().count(); t.reset();

    // find antinodes
    int const h = lines, w = length;
    std::unordered_set<int> set1, set2;
    for (const auto& it : map)
    {
        // for each pair of antennas find the antinodes
        // and insert their positions in the set
        const Array& positions = it.second;
        for (int i = 0; i < positions.size(); i++)
        for (int j = i+1; j < positions.size(); j++)
        {
            const Vec2& a = positions[i], b = positions[j];
            const Vec2 ab = b-a, p1 = a - ab, p2 = b + ab;
            // part 1, only two antinodes per pair of antennas
            if (p1.valid(w, h)) set1.insert(p1.index(w, h));
            if (p2.valid(w, h)) set1.insert(p2.index(w, h));
            // part 2, march along the line connecting a pair of antennas
            Vec2 tmp = a;
            while(tmp.valid(w, h)) { set2.insert(tmp.index(w, h)); tmp = tmp - ab; }
            tmp = b;
            while(tmp.valid(w, h)) { set2.insert(tmp.index(w, h)); tmp = tmp + ab; }
        }
    }
    const double t2 = t.micro().count(); t.reset();
    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu\n", set1.size());
    printf("Part 2: %zu\n", set2.size());
    printf("Time: %f µs\n", t2);
    return 0;
}