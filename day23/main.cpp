#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <bitset>
#include <unordered_set>
#include "../common/timer.h"

// AoC Day 23
// Part 1:
// Part 2:

// local types, we store vertices in a bitset of size 676 because that covers 
// all possible indices for nodes composed of two characters
constexpr size_t gsize = 26 * 26;
using Vertices = std::bitset<gsize>;
using Graph = std::array<Vertices, gsize>;
constexpr size_t start_t = 26 * int('t'-'a');
constexpr size_t end_t = 26 * int('u'-'a');

inline int solve(     Vertices&& R
                    , Vertices&& P
                    , Vertices&& X
                    , const Graph& graph
                    , std::vector<Vertices>& cliques
                    , int max = 0ul)
{
    if (P.none() && X.none()) 
    {
        const int count = R.count();
        if (count > max)
        {
            max = count;
            cliques.push_back(R);
        }
        return max;
    }
    // early quit if there remains not enough nodes to make
    // a clique greater than the current max 
    else if (P.count() + R.count() < max)
        return max;

    int id = 0; 
    while (P.any())
    {
        Vertices nextR = R, nextP, nextX;
        while(!P.test(id)) id++; // find first vertex in P

        nextR.set(id, true);
        for (int i = 0; i < gsize; ++i)
        {
            if (P.test(i) && graph[id].test(i)) nextP.set(i, true);
            if (X.test(i) && graph[id].test(i)) nextX.set(i, true);
        }
        const int count = solve(std::move(nextR), std::move(nextP), std::move(nextX), graph, cliques, max);
        if (count > max) max = count;

        P.set(id, false);
        X.set(id, true);
    }

    return max;
}

inline int  index(char a, char b)           { return int(b-'a') + 26 * int(a-'a'); }
inline char character(int id, bool first)   { return first ? char(id / 26 + 'a') : char(id % 26 + 'a'); }
inline bool test(int id)                    { return id >= start_t && id < end_t; }
inline int  make_edge(int a, int b)         { return a > b ? b + (a << 10) : a + (b << 10); }
inline int  make_tri(int a, int b, int c)
{
    int ids[3] = { a, b, c };
    std::sort(ids, ids+3);
    return ids[0] + (ids[1] << 10) + (ids[2] << 20);
}

std::string to_string(const Vertices& verts)
{
    std::string str;
    for (int i = 0; i < verts.size(); ++i)
    {
        if (verts.test(i))
        {
            str += character(i, true);
            str += character(i, false);
            str += ',';
        }
    }
    str.pop_back(); // remove the last comma
    return str;
}

int main(int argc, char * argv[])
{
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    Graph graph;
    Vertices verts;
    std::string line;
    while(std::getline(file, line))
    {
        char a, b, c, d;
        if (sscanf(line.c_str(), "%c%c-%c%c", &a, &b, &c, &d))
        {
            const int first = index(a, b), second = index(c, d);
            if (first != second)
            {
                graph[first][second] = graph[second][first] = true;
                verts.set(first, true);
                verts.set(second, true);
                // printf("%s: link %d - %d: %zu\n", line.c_str(), first, second, linkid);
            }
        }
    }

    // part1, fast version after abandoning the O(N^2) over edges one 
    const double t1 = t.micro().count(); t.reset();
    size_t part1 = 0ul;
    std::unordered_set<int> trio; // unique triangles

    // for each vertex starting with 't'
    for (int i = start_t; i < end_t; ++i)
    {
        if (!verts.test(i)) continue; // skip invalid vertices

        // now take a second vertex in its neighbors
        for (int j = 0; j < graph[i].size(); ++j)
        {
            if (!graph[i].test(j)) continue; // skip invalid neighbors

            // now take a third vertex in its neighbor's neighbors
            for (int k = 0; k < graph[j].size(); ++k)
            {
                if (!graph[j].test(k)) continue; // skip invalid neighbors

                // if i is a neighbor of k, we found a potential triangle
                if (graph[k].test(i))
                {
                    const size_t trioid = make_tri(i, j, k); 
                    auto it = trio.insert(trioid);
                    part1 += it.second;
                }
            }
        }
    }

    const double t2 = t.micro().count(); t.reset();

    // part 2: find clique of maximum size in the graph
    // use a modified B-K algorithm 
    // https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm
    std::vector<Vertices> cliques;
    cliques.reserve(4);
    solve({}, std::move(verts), {}, graph, cliques);
    const Vertices& maxclique = cliques.back();  // the max clique should always be the last one
    const double t3 = t.micro().count(); t.reset();

    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu, Time: %f µs\n", part1, t2);
    printf("Part 2: %s, Time: %f µs\n", to_string(maxclique).c_str(), t3);
}