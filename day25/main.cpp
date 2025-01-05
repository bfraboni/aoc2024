#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <bitset>
#include <unordered_set>
#include "../common/timer.h"

// AoC Day 25
// Part 1:
// Part 2:

using Format = std::array<std::string, 7>;
using Compact = std::array<int8_t, 5>;

void print(const Compact& c)
{
    printf("{ ");
    for (int i = 0; i < 5; ++i) printf("%d ", (int)c[i]);
    printf("}\n");
}

size_t hash(const Compact& c)
{
    size_t res = 0ul;
    for (int i = 0; i < c.size(); ++i)
        res += size_t(c[i]) << (i * 3); // only needs 3 bits per component
    return res;
}

void print(const Format& f)
{
    for (int i = 0; i < 7; ++i) std::cout << f[i] << std::endl;
}

bool iskey(const Format& f) { return f[0][0] == '.'; }
bool islock(const Format& f) { return f[0][0] == '#'; }

Compact compact(const Format& f)
{
    const bool key = iskey(f);
    Compact c;
    for (int i = 0; i < 5; ++i)
    {
        int j = key ? 5 : 1, count = 0;
        while (f[j][i] == '#')
        {
            count ++;
            j += key ? -1 : 1;
        }
        c[i] = count;
    }
    return c;
}


bool match(const Compact& lock, const Compact& key)
{
    for (int i = 0; i < 5; ++i)
        if (lock[i] + key[i] > 5)
            return false;
    return true;
}

int main(int argc, char * argv[])
{
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::vector<Compact> locks, keys;
    std::unordered_set<size_t> slocks, skeys;
    std::string line;
    Format f; int id = 0;
    while(std::getline(file, line))
    {
        if (line.empty()) continue;

        f[id++] = line;
        if (id == 7)
        {
            id = 0;
            const Compact c = compact(f);
            const bool key = iskey(f);
            const size_t hashed = hash(c);

            print(f);
            print(c);
            printf("%zu\n", hashed);

            if (key)
            {
                printf("new key added\n\n");
                keys.push_back(c);
                skeys.insert(hashed);
            }
            else
            {
                printf("new lock added\n\n");
                locks.push_back(c);
                slocks.insert(hashed);
            }
        }
    }
    printf("keys %zu locks %zu\n", keys.size(), locks.size());
    printf("set keys %zu set locks %zu\n", skeys.size(), slocks.size());

    const double t1 = t.micro().count(); t.reset();

    size_t part1 = 0ul;
    for (int i = 0; i < locks.size(); ++i)
        for (int j = 0; j < keys.size(); ++j)
            part1 += match(locks[i], keys[j]);

    const double t2 = t.micro().count(); t.reset();

    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu, Time: %f µs\n", part1, t2);
    // printf("Part 2: %s, Time: %f µs\n", to_string(maxclique).c_str(), t3);
}