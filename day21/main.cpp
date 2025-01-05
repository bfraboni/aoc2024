#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include "../common/timer.h"

// AoC Day 21
// Part 1:
// Part 2:

enum Dir : int8_t { Up = 0, Right = 1, Down = 2, Left = 3 };
enum Order : int8_t { None = 0, VerticalFirst, HorizontalFirst };

struct Cell
{
    int8_t x, y, v, id;
    inline Cell() {}
    inline Cell(int8_t x, int8_t y, int8_t v, int8_t id) : x(x), y(y), v(v), id(id) {}
    inline int index(const int w, const int h) const { return y * w + x; }
    inline bool valid(const int w, const int h) const { return x >= 0 && x < w && y >= 0 && y < h; }
};

// Numerical keypad layout
// +---+---+---+
// | 7 | 8 | 9 |
// +---+---+---+
// | 4 | 5 | 6 |
// +---+---+---+
// | 1 | 2 | 3 |
// +---+---+---+
//     | 0 | A | <- start pointing at A
//     +---+---+
static const Cell numpad[11] = {
    { 0, 0, 7, 0 },
    { 1, 0, 8, 1 },
    { 2, 0, 9, 2 },
    { 0, 1, 4, 3 },
    { 1, 1, 5, 4 },
    { 2, 1, 6, 5 },
    { 0, 2, 1, 6 },
    { 1, 2, 2, 7 },
    { 2, 2, 3, 8 },
    // unused
    { 1, 3, 0, 10 },
    { 2, 3,-1, 11 }
};

// Directional keypad layout
//     +---+---+
//     | ^ | A |
// +---+---+---+
// | < | v | > | <- start pointing at A
// +---+---+---+
static const Cell dirpad[5] = {
    // unused
    { 1, 0, Up,     1},
    { 2, 0, -1,     2 },
    { 0, 1, Left,   3 },
    { 1, 1, Down,   4 },
    { 2, 1, Right,  5 }
};

struct Route
{
    int8_t dx, dy;
    Order order;
};

// the fastest way from one cell to the other is to always trace 2 lines so only two directions are required
//! NOTE: this should work for both numpad and dirpad
inline Route route(const Cell& a, const Cell& b)
{
    if (a.v <= 0 && b.x == 0) // [A-0] to [1st col] needs straight vertical moves first
        return { int8_t(b.x - a.x), int8_t(b.y - a.y), VerticalFirst };
    else if (a.x == 0 && b.v <= 0) // [1st col] to [A-0] needs straight horizontal moves first
        return { int8_t(b.x - a.x), int8_t(b.y - a.y), HorizontalFirst };
    // [1-9] to [1-9], [0] to [A], [A] to [0] can be applied in any order
    return { int8_t(b.x - a.x), int8_t(b.y - a.y), None };
}

// given a route, make sure we can build quickly a string of corresponding moves
static constexpr std::string xmoves[5] = { ">>", ">", "", "<", "<<" };
static constexpr std::string ymoves[7] = { "vvv", "vv", "v", "", "^", "^^", "^^^" };
static const std::vector<std::string> yxmoves = [](){
    std::vector<std::string> vec(7 * 5);
    int id = 0;
    for (const auto& y : ymoves)
        for (const auto& x : xmoves)
            vec[id++] = y + x + 'A';
    return vec;
}();
static const std::vector<std::string> xymoves = [](){
    std::vector<std::string> vec(7 * 5);
    int id = 0;
    for (const auto& y : ymoves)
        for (const auto& x : xmoves)
            vec[id++] = x + y + 'A';
    return vec;
}();
inline const std::string& stringify(const Route& r, const Order order = None)
{
    const int8_t idx = (3-r.dy) * 5 + 2-r.dx; 
    return order == VerticalFirst ? yxmoves[idx] : xymoves[idx];
}
static const std::unordered_map<size_t, std::string> numroutes = [](){
    std::unordered_map<size_t, std::string> map;
    for (const auto& a : numpad)
    for (const auto& b : numpad)
    {
        const Route r = route(a, b);
        const size_t hash1 = 0 + (int(a.id) << 8) + (int(b.id) << 16);
        const size_t hash2 = 1 + (int(a.id) << 8) + (int(b.id) << 16);
        if (r.order == None)
        {
            map[hash1] = stringify(r, HorizontalFirst);
            map[hash2] = stringify(r, VerticalFirst);
        }
        else
            map[hash1] = stringify(r, r.order);
    }
    return map;
}();
static const std::unordered_map<size_t, std::string> dirroutes = [](){
    std::unordered_map<size_t, std::string> map;
    for (const auto& a : dirpad)
    for (const auto& b : dirpad)
    {
        const Route r = route(a, b);
        const size_t hash1 = 0 + (int(a.id) << 8) + (int(b.id) << 16);
        const size_t hash2 = 1 + (int(a.id) << 8) + (int(b.id) << 16);
        if (r.order == None)
        {
            map[hash1] = stringify(r, HorizontalFirst);
            map[hash2] = stringify(r, VerticalFirst);
        }
        else
            map[hash1] = stringify(r, r.order);
    }
    return map;
}();

inline const Cell& numcell(const char& c)
{
    if      (c == '7') return numpad[0];
    else if (c == '8') return numpad[1];
    else if (c == '9') return numpad[2];
    else if (c == '4') return numpad[3];
    else if (c == '5') return numpad[4];
    else if (c == '6') return numpad[5];
    else if (c == '1') return numpad[6];
    else if (c == '2') return numpad[7];
    else if (c == '3') return numpad[8];
    else if (c == '0') return numpad[9];
    else               return numpad[10];
}

inline const Cell& dircell(const char& c)
{
    if      (c == '^') return dirpad[0];
    else if (c == 'A') return dirpad[1];
    else if (c == '<') return dirpad[2];
    else if (c == 'v') return dirpad[3];
    else               return dirpad[4];
}

inline size_t solve(std::string seq, int8_t depth, std::unordered_map<size_t, size_t>& cache)
{
    if (depth == 0) { printf("solved %s \n", seq.c_str()); return seq.size(); }

    size_t res = 0;
    seq = 'A' + seq;
    for (int i = 0; i < seq.size()-1; i++)
    {
        const Cell& a = dircell(seq[i]);
        const Cell& b = dircell(seq[i+1]);
        const int ia = a.id; //index(3, 2);
        const int ib = b.id; //index(3, 2);
        const size_t hash = depth + (ia << 8) + (ib << 16);
        if (!cache.contains(hash))
        {
            // const Route r = route(a, b);
            const size_t hash1 = 0 + (int(a.id) << 8) + (int(b.id) << 16);
            const size_t hash2 = 1 + (int(a.id) << 8) + (int(b.id) << 16);
            if (dirroutes.contains(hash2))
            {
                cache[hash] = std::min(
                    solve(dirroutes.at(hash1), depth-1, cache),
                    solve(dirroutes.at(hash2), depth-1, cache)
                );
            }
            else
            {
                cache[hash] = solve(dirroutes.at(hash1), depth-1, cache);
            }
        }
        res += cache[hash];
    }
    return res;
}

// part 1:
// - Door numpad
// - Robot1 dirpad
// - Robot2 dirpad
// - Our dirpad
int main(int argc, char * argv[])
{   
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::string line;
    size_t part1 = 0ul, part2 = 0ul;
    std::unordered_map<size_t, size_t> cache;
    while(std::getline(file, line))
    {
        std::istringstream iss(line);
        size_t num; iss >> num;
        line = 'A' + line;
        // printf("we need to type %s\n", line.c_str());
        size_t best1 = 0, best2 = 0ul;
        for (int i = 0; i < line.size()-1; i++)
        {
            const Cell& a = numcell(line[i]);
            const Cell& b = numcell(line[i+1]);
            // const Route r = route(a, b);
            const size_t hash1 = 0 + (int(a.id) << 8) + (int(b.id) << 16);
            const size_t hash2 = 1 + (int(a.id) << 8) + (int(b.id) << 16);
            // printf("%c -> %c :\n", line[i], line[i+1]);
            // if (r.order == None)
            if (numroutes.contains(hash2))
            {
                // const size_t v1 = solve(numroutes.at(hash1), 25, cache);
                // const size_t v2 = solve(numroutes.at(hash2), 25, cache);
                // printf(" @25 #1 %s -> %zu \n", numroutes.at(hash1).c_str(), v1);
                // printf(" @25 #2 %s -> %zu \n", numroutes.at(hash2).c_str(), v2);
                // const size_t v11 = solve(numroutes.at(hash1), 2, cache);
                // const size_t v22 = solve(numroutes.at(hash2), 2, cache);
                // printf(" @2 #1 %s -> %zu \n", numroutes.at(hash1).c_str(), v11);
                // printf(" @2 #2 %s -> %zu \n", numroutes.at(hash2).c_str(), v22);


                best2 += std::min(
                    solve(numroutes.at(hash1), 25, cache),
                    solve(numroutes.at(hash2), 25, cache)
                );
                best1 += std::min(
                    solve(numroutes.at(hash1), 2, cache),
                    solve(numroutes.at(hash2), 2, cache)
                );
            }
            else
            {
                best2 += solve(numroutes.at(hash1), 25, cache);
                best1 += solve(numroutes.at(hash1), 2, cache);
            }
        }
        part1 += best1 * num;
        part2 += best2 * num;
    }

    const double t1 = t.micro().count(); t.reset();
    
    printf("Time: %f µs\n", t1);
    printf("Part 1: %zu\n", part1);
    printf("Part 2: %zu\n", part2);
}