#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include "../common/timer.h"

// AoC Day 6
// Part 1: The guard '^' can only move forward and turn right when blocked '#'. 
//         Count all distinct positions of the guard before he leaves the map.
// Part 2: For each unique position except the initial one, add an obstacle and
//         test if the guard make a loop. Count the number of loops.

struct Vec2
{
    static constexpr int dx[4] = { 0, 1, 0, -1 }, dy[4] = { -1, 0, 1, 0 };
    int x, y, dir;
    Vec2() {}
    Vec2(int x, int y, int dir = 0) : x(x), y(y), dir(dir) {}
    Vec2 next() const { return Vec2(x+dx[dir], y+dy[dir], dir); }
    Vec2 turn() const { return Vec2(x, y, (dir+1)%4); }
    int index(const int w, const int h) const { return y * w + x; }
    int hash(const int w, const int h) const { return y * w * 4 + x * 4 + dir; }
    bool valid(const int w, const int h) const { return x >= 0 && x < w && y >= 0 && y < h; }
    void print_pos() const { std::cout << "(" << x << " " << y << ")"; }
    void print_dir() const { std::cout << "(" << dx[dir] << " " << dy[dir] << ")"; }
};

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
    int const h = data.size(), w = data[0].size();

    // Part 1
    const double t1 = t.micro().count(); t.reset();
    std::unordered_set<int> set1;
    set1.insert(guard.index(w, h));
    Vec2 current = guard;
    auto obstacle = [&](const Vec2& p) { return data[p.y][p.x] == '#'; };
    while(current.valid(w, h))
    {
        const Vec2 next = current.next();
        if (!next.valid(w, h))      break;
        else if (obstacle(next))    current = current.turn();
        else                        current = next;
        set1.insert(current.index(w, h));
    }

    // Part2
    //! TOFIX: this is pretty slow..
    const double t2 = t.micro().count(); t.reset();
    size_t part2 = 0;
    std::unordered_set<int> set2;
    for (const int& index : set1)
    {
        // printf("index %d\n", index);
        Vec2 current = guard;
        if (current.index(w, h) == index) continue; // we do not test for the initial position

        // clear set and add initial position as visited
        set2.clear();
        set2.insert(current.hash(w, h));
        // Add obstacle at index position and test if the guard is stuck in a loop
        auto obstacle = [&](const Vec2& p) { return data[p.y][p.x] == '#' || p.index(w, h) == index; };
        while(current.valid(w, h))
        {
            const Vec2 next = current.next();
            if (!next.valid(w, h))      break;
            else if (obstacle(next))    current = current.turn();
            else                        current = next;

            // we already visited the position, this makes a loop
            auto res = set2.insert(current.hash(w, h));
            if (!res.second) 
            {
                part2++;
                break;
            }
        }
    }
    const double t3 = t.micro().count();

    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu Time: %f µs\n", set1.size(), t2);
    printf("Part 2: %zu Time: %f µs\n", part2, t3);
    return 0;
}