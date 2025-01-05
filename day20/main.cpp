#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <queue>
#include <unordered_set>
#include <unordered_map>
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
    int prev     = -1;
};

struct Compare
{
    std::vector<Node>& nodes;
    Compare(std::vector<Node>& nodes) : nodes(nodes) {}
    inline bool operator()(const int a, const int b) const
    {
        return nodes[a].cost > nodes[b].cost;
    }
};

inline size_t solve(const std::vector<std::string>& map, const Vec2 start, const Vec2 end)
{
    const int w = map[0].size(), h = map.size();
    const int targetid = end.index(w, h);
    std::vector<Node> nodes(w * h);
    Compare comp(nodes);
    std::vector<int> container; container.reserve(256);
    std::priority_queue<int, std::vector<int>, Compare> queue(comp, container);

    // init
    const int startid = start.index(w, h);
    nodes[startid] = { start, 0, 0ul, -1 };
    queue.push(startid);

    // BFS
    int bestcost = 1 << 30;
    while(!queue.empty())
    {
        Node& n = nodes[queue.top()];
        queue.pop();

        const int id = n.p.index(w, h);
        if (id == targetid)
        {
            bestcost = n.cost;
            break;
        }
        n.visit = true;

        auto process = [&](const Vec2& next, const int cost)
        {
            const bool valid = next.valid(w, h);
            const bool track = map[next.y][next.x] != '#';
            if (valid && track)
            {
                const int nid = next.index(w, h);
                if (n.cost + cost < nodes[nid].cost)
                {
                    nodes[nid] = { next, nodes[nid].visit, n.cost + cost, id };
                    queue.push(nid);
                }
            }
        };

        process(n.p.next(Up),     1);
        process(n.p.next(Left),   1);
        process(n.p.next(Down),   1);
        process(n.p.next(Right),  1);
    }

    if (bestcost != 1<<30)
    {
        printf("best route cost: %d id %d\n" , bestcost, targetid);

        std::vector<int> route(bestcost+1, targetid);

        // for (int i = 0; i<route.size(); ++i)
        // {
        //     const Node& ni = nodes[route[i]];
        //     printf("node: num %d id %d pos (%d, %d) cost %d\n", i, route[i], ni.p.x, ni.p.y, ni.cost);
        // }
        
        const Node& n = nodes[targetid];
        int rid = bestcost-1;
        // printf("id: %d %d (%d, %d)\n", bestcost, targetid, n.p.x, n.p.y);
        // auto copy = map;
        // copy[n.p.y][n.p.x] = '+';
        while(rid >= 0)
        {
            const Node& n = nodes[route[rid+1]];
            // printf("id: %d %d (%d, %d)\n", rid, n.prev, nodes[n.prev].p.x, nodes[n.prev].p.y);
            route[rid--] = n.prev;
            // copy[n.p.y][n.p.x] = '+';
        }

        // for (int i = 0; i<route.size(); ++i)
        // {
        //     const Node& ni = nodes[route[i]];
        //     printf("node: num %d id %d pos (%d, %d) cost %d\n", i, route[i], ni.p.x, ni.p.y, ni.cost);
        // }

        // for(auto& s: copy)
        //     std::cout << s<< std::endl;

        constexpr int moves = 20, save = 100;
        int count = 0;
        // std::unordered_set<size_t> set;
        // std::unordered_map<size_t, size_t> map;
        for (int i = 0; i < route.size()-save; ++i)
        {
            for (int j = i+save; j < route.size(); ++j)
            {
                const Node& ni = nodes[route[i]], nj = nodes[route[j]];
                const int dij = std::abs(ni.p.x - nj.p.x) + std::abs(ni.p.y - nj.p.y);
                const int cij = nj.cost - ni.cost;

                // if (i == 18 && j == 84)
                // {

                //     printf("start: num %d id %d pos (%d, %d) cost %d\n", i, route[i], ni.p.x, ni.p.y, ni.cost);
                //     printf("end:   num %d id %d pos (%d, %d) cost %d\n", j, route[j], nj.p.x, nj.p.y, nj.cost);
                //     printf("cheat: dist %d save %d\n", dij, cij-dij);
                //     // printf("cheat: (%d %d) %d -> (%d %d) %d ; %d %d\n", ni.p.x, ni.p.y, ni.cost, nj.p.x, nj.p.y, nj.cost, dij, cij);
                // }

                if (dij <= moves && cij >= save + dij)
                {
                    count++;
                    // const size_t a = ni.p.index(w, h);
                    // const size_t b = nj.p.index(w, h);
                    // const size_t hash = (a << 32) + b;
                    // auto it = set.emplace(hash);
                    // if (!it.second)
                    //     printf("duplicate: (%d %d) -> (%d %d)\n", ni.p.x, ni.p.y, nj.p.x, nj.p.y);

                    // map[cij-dij]+=1;
                }
            }
        }

        // for (auto& it : map)
        // {
        //     std::cout << it.second  << " cheats that save " << it.first << " picoseconds " << std::endl;
        // }
        // printf("potential cheat routes saving 100 cost: %d %d\n", count, (int)set.size());
        printf("potential cheat routes saving 100 cost: %d \n", count);
    }

    // count the number of short

    return bestcost;
}

int main(int argc, char * argv[])
{   
    Timer t;
#if 1
    std::fstream file("input.txt", std::ios_base::in);
#else
    std::string filedata = "###############\n#...#...#.....#\n#.#.#.#.#.###.#\n#S#...#.#.#...#\n#######.#.#.###\n#######.#.#...#\n#######.#.###.#\n###..E#...#...#\n###.#######.###\n#...###...#...#\n#.#####.#.###.#\n#.#...#.#.#...#\n#.#.#.#.#.#.###\n#...#...#...###\n###############\n";
    std::stringstream file(filedata);
#endif
    std::string line;
    std::vector<std::string> map;
    Vec2 start, end;
    while(std::getline(file, line))
    {
        map.push_back(line);
        size_t pos;
        if ((pos = line.find('E')) != std::string::npos) end   = Vec2(pos, map.size()-1);
        if ((pos = line.find('S')) != std::string::npos) start = Vec2(pos, map.size()-1);
    }

    const double t1 = t.micro().count(); t.reset();
    const size_t part1 = solve(map, start, end);
    // const size_t part1 = 0ul;
    const double t2 = t.micro().count(); t.reset();
    
    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu, Time: %f µs\n", part1, t2);
    printf("Part 2: %zu, Time: %f µs\n", 0ul, t2);
}