#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include "../common/timer.h"

// AoC Day 11
// Part 1: How many stone after blinking 25 times ?
// Part 2: How many stone after blinking 75 times ? (naive version explodes in complexity)

// fast

std::pair<size_t, size_t> solve(const std::vector<size_t>& data, const std::pair<size_t, size_t>& iterations)
{
    std::unordered_map<size_t, size_t> mapa, mapb;
    for (auto i : data) mapa[i] += 1ul;

    std::pair<size_t, size_t> result = {0ul, 0ul};
    for (int i = 0; i < iterations.second; ++i)
    {
        auto& in = i%2==0 ? mapa : mapb;
        auto& out = i%2==0 ? mapb : mapa;
        out.clear();
        size_t total = 0;
        for (const auto& p : in)
        {
            //! rule #1
            if (p.first == 0ul)
            {
                out[1ul] += p.second;
                total += p.second;
            }
            else
            {
                //! rule #2
                const size_t digits = std::log10(p.first) + 1ul;
                if (digits%2 == 0)
                {
                    size_t split = std::pow(10ul, digits/2);
                    out[p.first/split] += p.second;
                    out[p.first%split] += p.second;
                    total += 2ul * p.second;
                }
                //! rule #3
                else
                {
                    out[p.first * 2024ul] += p.second;
                    total += p.second;
                }
            }
        }

        if (i == iterations.first-1) result.first = total;
        else if (i == iterations.second-1) result.second = total;
    }

    return result;
}

int main(int argc, char * argv[])
{   
    // read all data at once
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::vector<size_t> data;
    size_t a;
    while(file >> a) data.push_back(a);
    const std::pair<size_t, size_t> res = solve(data, {25, 75}); 
    const double t1 = t.micro().count(); t.reset();
    printf("Part 1: %zu\n", res.first);
    printf("Part 2: %zu\n", res.second);
    printf("Time: %f µs\n", t1);
    return 0;
}