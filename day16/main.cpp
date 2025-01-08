#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cassert>
#include <cstring>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include "../common/timer.h"

// AoC Day 16: https://adventofcode.com/2024/day/16
// Part 1:
// Part 2:

enum Dir : int { North = 0, East = 1, South = 2, West = 3 };

struct Vec2
{
    static constexpr int dx[4] = { 0, 1, 0, -1 }, dy[4] = { -1, 0, 1, 0 };
    int x, y;
    Dir d;
    inline Vec2() {}
    inline Vec2(int x, int y, Dir d = North) : x(x), y(y), d(d) {}

    inline Vec2 next()     const { return Vec2(x+dx[d], y+dy[d], d); }
    inline Vec2 turn_cw()  const { return Vec2(x, y, Dir((d-1+4)%4)); }
    inline Vec2 turn_ccw() const { return Vec2(x, y, Dir((d+1)%4)); }

    inline int index(const int w, const int h) const { return y * w + x; }
    inline int hash (const int w, const int h) const { return 4 * (y * w + x) + d; }
};

struct Node
{
    Vec2 p;
    int cost        = 1 << 30;
    int visit       = 0;
    int psize       = 0;
    int prevs[6]    = {-1};

    inline void add(const int n) { prevs[psize++] = n; }
    inline void reset() { psize = 0; }
    inline void reset_and_add(const int n) { reset(); add(n); }
    inline void add_if_not_find(const int n)
    {
        if (!std::any_of(prevs, prevs+psize, [&](int a){ return a == n; }))
            add(n);
    }
};

struct Compare
{
    std::vector<Node>& nodes;
    Compare(std::vector<Node>& nodes) : nodes(nodes) {}
    inline bool operator()(const int a, const int b) const { return nodes[a].cost > nodes[b].cost; }
};

inline size_t backtrack(const int current, const int w, const int h, const std::vector<Node>& nodes)
{
    static std::unordered_set<int> best_nodes; // this makes me a dummy use-once function
    best_nodes.insert(nodes[current].p.index(w, h));
    if (nodes[current].cost > 0)
        for (int i = 0; i < nodes[current].psize; ++i)
            backtrack(nodes[current].prevs[i], w, h, nodes);
    return best_nodes.size();
}

inline std::pair<size_t, size_t> solve(const std::vector<std::string>& map, const Vec2 start, const Vec2 end)
{
    const int w = map[0].size(), h = map.size();
    std::vector<Node> nodes(w * h * 4);
    Compare comp(nodes);
    std::vector<int> container; container.reserve(256);
    std::priority_queue<int, std::vector<int>, Compare> queue(comp, container);

    // init
    const int startid = start.hash(w, h);
    nodes[startid] = { start, 0, 0 };
    queue.push(startid);

    // BFS
    int bestcost = 1 << 30, best = -1;
    while(!queue.empty())
    {
        Node& n = nodes[queue.top()];
        queue.pop();

        const int hash = n.p.hash(w, h);
        if (n.p.index(w, h) == end.index(w, h))
        {
            bestcost = n.cost;
            best = hash;
            break;
        }
        n.visit = true;

        auto process = [&](const Vec2& next, const int cost) {
            const int nhash = next.hash(w, h);
            const bool validnext = map[next.y][next.x] != '#';
            if (validnext)
            {
                if (n.cost + cost < nodes[nhash].cost)
                {
                    nodes[nhash] = { next, n.cost + cost, nodes[nhash].visit };
                    // we found a better route, reset list of previous best nodes
                    nodes[nhash].reset_and_add(hash);
                    if (!nodes[nhash].visit)
                        queue.push(nhash);
                }
                else if (n.cost + cost == nodes[nhash].cost)
                {
                    // update list of previous best nodes
                    nodes[nhash].add_if_not_find(hash);
                }
            }
        };

        process(n.p.next(),     1);     // go forward
        process(n.p.turn_cw(),  1000);  // turn clockwise
        process(n.p.turn_ccw(), 1000);  // turn counter clockwise
    }
    assert(best > 0);

    // find all nodes that are part of a best route by backtracking previous nodes
    const size_t best_nodes = backtrack(best, w, h, nodes);
    return { bestcost, best_nodes };
}

int main(int argc, char * argv[])
{
    // read all data at once
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::string line;
    std::vector<std::string> map;
    Vec2 start, end;
    while(std::getline(file, line))
    {
        map.push_back(line);
        size_t pos;
        if ((pos = line.find('E')) != std::string::npos) end   = Vec2(pos, map.size()-1);
        if ((pos = line.find('S')) != std::string::npos) start = Vec2(pos, map.size()-1, East);
    }

    const double t1 = t.micro().count(); t.reset();
    const std::pair<size_t, size_t> res = solve(map, start, end);
    const double t2 = t.micro().count(); t.reset();
    
    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu\n", res.first);
    printf("Part 2: %zu\n", res.second);
    printf("Time: %f µs\n", t2);
    return 0;
}