#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "../common/timer.h"

// AoC Day 25
// Part 1: 
// Part 2: Done with AoC until next year !!!

using Format    = std::array<std::string, 7>;
using Compact   = std::array<int8_t, 5>;
using Array     = std::vector<Compact>;

inline bool iskey(const Format& f) { return f[0][0] == '.'; }

template<bool key>
inline Compact compact(const Format& f)
{
    Compact c;
    for (int i = 0; i < 5; ++i)
    {
        c[i] = 0;
        int j = key ? 5 : 1;
        while (f[j][i] == '#')
        {
            c[i]++;
            j += key ? -1 : 1;
        }
    }
    return c;
}

inline bool match(const Compact& lock, const Compact& key)
{
    for (int i = 0; i < 5; ++i)
        if (lock[i] + key[i] > 5)
            return false;
    return true;
}

inline size_t solve(const Array& locks, const Array& keys)
{
    size_t res = 0ul;
    for (int i = 0; i < locks.size(); ++i)
        for (int j = 0; j < keys.size(); ++j)
            res += match(locks[i], keys[j]);
    return res;
}

int main(int argc, char * argv[])
{
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    Array locks, keys;
    std::string line;
    Format f; int id = 0;
    while(std::getline(file, line))
    {
        if (line.empty()) continue;

        f[id++] = line;
        if (id == 7)
        {
            id = 0;
            if (iskey(f))
                keys.emplace_back(compact<true>(f));
            else
                locks.emplace_back(compact<false>(f));
        }
    }

    const double t1 = t.micro().count(); t.reset();
    const size_t part1 = solve(locks, keys);
    const double t2 = t.micro().count(); t.reset();

    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu, Time: %f µs\n", part1, t2);
    printf("Part 2: Done with AoC until next year !!!\n");
}