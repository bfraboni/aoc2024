#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include "../common/timer.h"

// AoC Day 11: https://adventofcode.com/2024/day/11
// Part 1: How many stone after blinking 25 times ?
// Part 2: How many stone after blinking 75 times ? (naive version explodes in complexity, needs caching)

template <size_t N, size_t P>
struct Pow { static constexpr size_t value = N * Pow<N, P - 1>::value; };
template <size_t N>
struct Pow<N, 0> { static constexpr size_t value = 1; };

static constexpr size_t pow10[21] = {
    Pow<10ul, 0ul>::value,
    Pow<10ul, 1ul>::value,
    Pow<10ul, 2ul>::value,
    Pow<10ul, 3ul>::value,
    Pow<10ul, 4ul>::value,
    Pow<10ul, 5ul>::value,
    Pow<10ul, 6ul>::value,
    Pow<10ul, 7ul>::value,
    Pow<10ul, 8ul>::value,
    Pow<10ul, 9ul>::value,
    Pow<10ul, 10ul>::value, // greater than 2^31 and 2^32 which should be enough for int32 and uint32
    Pow<10ul, 11ul>::value,
    Pow<10ul, 12ul>::value,
    Pow<10ul, 13ul>::value,
    Pow<10ul, 14ul>::value,
    Pow<10ul, 15ul>::value,
    Pow<10ul, 16ul>::value,
    Pow<10ul, 17ul>::value,
    Pow<10ul, 18ul>::value,
    Pow<10ul, 19ul>::value, // greater than 2^63 which should be enough for int64
    Pow<10ul, 20ul>::value  // greater than 2^64 which should be enough for uint64
};

inline constexpr size_t quick_pow10(const size_t n) { return pow10[n]; }

inline constexpr size_t quick_digits(const size_t v)
{
    // if (!v) return 1ul; // if ones need to handle zero
    size_t res = 0ul;
    while(v >= pow10[res++]);
    return res-1;
}

std::pair<size_t, size_t> solve(const std::vector<size_t>& data, const std::pair<size_t, size_t>& iterations)
{
    std::pair<size_t, size_t> result = {0ul, 0ul};

    // init map with stone numbers
    std::unordered_map<size_t, size_t> mapa, mapb;
    for (const auto& i : data) mapa[i] += 1ul;

    for (int i = 0; i < iterations.second; ++i)
    {
        auto& in = i%2==0 ? mapa : mapb;
        auto& out = i%2==0 ? mapb : mapa;
        out.clear();
        size_t stones = 0;
        // apply rules and update stone count in the output map
        for (const auto& p : in)
        {
            //! rule #1
            if (p.first == 0ul)
            {
                out[1ul] += p.second;
                stones += p.second;
            }
            else
            {
                //! rule #2
                const size_t digits = quick_digits(p.first);
                if (digits%2 == 0)
                {
                    size_t split = quick_pow10(digits/2);
                    out[p.first/split] += p.second;
                    out[p.first%split] += p.second;
                    stones += 2ul * p.second;
                }
                //! rule #3
                else
                {
                    out[p.first * 2024ul] += p.second;
                    stones += p.second;
                }
            }
        }

        if (i == iterations.first-1) result.first = stones;
        else if (i == iterations.second-1) result.second = stones;
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
    const double t1 = t.micro().count(); t.reset();
    const std::pair<size_t, size_t> res = solve(data, {25, 75}); 
    const double t2 = t.micro().count(); t.reset();
    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu\n", res.first);
    printf("Part 2: %zu\n", res.second);
    printf("Time: %f µs\n", t2);
    return 0;
}