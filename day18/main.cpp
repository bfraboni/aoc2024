#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <queue>
#include "../common/timer.h"

// AoC Day 18
// Part 1:
// Part 2:

enum Dir : int { Up = 0, Right = 1, Down = 2, Left = 3 };
struct Vec2
{
    static constexpr int dx[4] = { 0, 1, 0, -1 }, dy[4] = { -1, 0, 1, 0 };
    int x, y;
    inline Vec2() {}
    inline Vec2(int x, int y) : x(x), y(y) {}
    inline int index(const int w, const int h) const { return y * w + x; }
    inline bool valid(const int w, const int h) const { return x >= 0 && x < w && y >= 0 && y < h; }
    inline Vec2 next(Dir d) const { return Vec2(x+dx[d], y+dy[d]); }
    bool operator==(const Vec2& other) const { return x==other.x && y==other.y; }
};

struct Node
{
    Vec2 p;
    int visit    = 0;
    int cost     = 1 << 30;
};

struct Compare
{
    std::vector<Node>& nodes;
    Compare(std::vector<Node>& nodes) : nodes(nodes) {}
    inline bool operator()(const int a, const int b) const { return nodes[a].cost > nodes[b].cost; }
};

inline size_t solve(const std::vector<std::string>& map)
{
    constexpr int w = 71, h = 71;
    const Vec2 start(0, 0), end(70, 70);
    const int targetid = end.index(w, h);
    std::vector<Node> nodes(w * h);
    Compare comp(nodes);
    std::vector<int> container; container.reserve(256);
    std::priority_queue<int, std::vector<int>, Compare> queue(comp, container);

    // init
    const int startid = start.index(w, h);
    nodes[startid] = { start, 0, 0ul };
    queue.push(startid);

    // BFS
    int bestcost = 1 << 30, best = -1;
    while(!queue.empty())
    {
        Node& n = nodes[queue.top()];
        queue.pop();

        const int id = n.p.index(w, h);
        if (id == targetid)
        {
            bestcost = n.cost;
            best = id;
            break;
        }
        n.visit = true;

        auto process = [&](const Vec2& next, const int cost)
        {
            const bool validnext = next.valid(w, h) && map[next.y][next.x] != '#';
            if (validnext)
            {
                const int nid = next.index(w, h);
                if (n.cost + cost < nodes[nid].cost)
                {
                    nodes[nid] = { next, nodes[nid].visit, n.cost + cost };
                    queue.push(nid);
                }
            }
        };

        process(n.p.next(Up),     1);
        process(n.p.next(Left),   1);
        process(n.p.next(Down),   1);
        process(n.p.next(Right),  1);
    }

    return bestcost;
}

int main(int argc, char * argv[])
{   
    // read all data at once
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::string line;
    std::vector<Vec2> coords;
    while(std::getline(file, line))
    {
        std::istringstream iss(line);
        Vec2 pos; char c;
        iss >> pos.x >> c >> pos.y;
        coords.push_back(pos);
    }

    constexpr int w = 71, h = 71;
    std::vector<std::string> memory(h, std::string(w, '.'));
    for (int i = 0; i < 1024; ++i)
        memory[coords[i].y][coords[i].x] = '#';
    
    const double t1 = t.micro().count(); t.reset();
    const size_t part1 = solve(memory);
    const double t2 = t.micro().count(); t.reset();

    // binary search the first non working byte
    int a = 1024, b = coords.size();
    while(a + 1 < b)
    {
        const int m = (a+b)/2;
        // init map up to m-th byte
        memory = std::vector<std::string>(h, std::string(w, '.'));
        for (int i = 0; i < m; ++i)
            memory[coords[i].y][coords[i].x] = '#';

        constexpr int fail = 1 << 30;
        if (solve(memory) == fail)
        {
            b = m;
        }
        else
        {
            a = m;
        }
    }
    const size_t part2 = a;
    const double t3 = t.micro().count(); t.reset();
    
    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu, Time: %f µs\n", part1, t2);
    printf("Part 2: %d,%d (%zu), Time: %f µs\n", coords[part2].x, coords[part2].y, part2, t3);
    return 0;
}