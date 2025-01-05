

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include "../common/timer.h"

// AoC Day 22
// Part 1:
// Part 2:

struct PRNG
{
    size_t state;

    PRNG(size_t seed) : state(seed){}

    inline void init(size_t seed) { state = seed; }

    inline size_t next()
    {
        state ^= (state << 6);
        state = state % 16777216;
        state ^= (state >> 5);
        state = state % 16777216;
        state ^= (state << 11);
        state = state % 16777216;
        return state;
    }
};

int main(int argc, char * argv[])
{   
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    size_t part1 = 0ul, num, best = 0ul /* , best_hash = -1 */;
    std::unordered_map<int, int> map;
    std::unordered_set<int> set;
    map.reserve(65536);
    set.reserve(2048);
    while(file >> num)
    {   
        PRNG prng(num);
        int price = num % 10, diffs[4];
        set.clear();
        for (int i = 0; i < 2000; ++i)
        {
            // advance prng state
            prng.next();
            const int new_price = prng.state % 10;
            // max diff is + 9, min diff is - 9, so 18 possibilities hence fit in 5 bits
            // adding 9 makes the difference always positive
            diffs[i%4] = 9 + new_price - price;
            price = new_price;
            // printf("%zu: %zd (%zd)\n", prng.state, new_price, diffs[i%4]);
            if (i >= 3) // circular buffer is full now
            {
                // compute unique hash from the 4 positive diffs index
                const int hash =    (diffs[(i-3)%4]      )
                                  + (diffs[(i-2)%4] << 8 )
                                  + (diffs[(i-1)%4] << 16)
                                  + (diffs[(i  )%4] << 24);

                // update hashmap with the number of bananas we'd sell if we'd stop after that sequence,
                // only if that sequence wasn't already seen (otherwise we'd have sold the bananas already)
                if (!set.contains(hash))
                {
                    set.insert(hash);
                    map[hash] += new_price;
                    if (map.at(hash) > best)
                        best = map.at(hash);
                }
            }
        }
        part1 += prng.state;
    }
    const size_t part2 = best;
    const double t1 = t.micro().count(); t.reset();
    
    printf("Time: %f µs\n", t1);
    printf("Part 1: %zu\n", part1);
    printf("Part 2: %zu\n", part2);
}