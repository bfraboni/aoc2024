#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "../common/timer.h"

// AoC Day 7
// Part 1: Compute the operation tree for plus and mult and test that one of the leave equals the expected result.
// Part 2: Add the concatenate operation.

size_t concatenate(size_t a, size_t b)
{
    const size_t digits = std::log10(b) + 1ul;
    return a * std::pow(10ul, digits) + b;
}

struct Result { bool a, b; };

template<bool concat>
void test(bool& res, const size_t value, const size_t search, const std::vector<size_t>& data, const int depth)
{
    const bool leaf = depth == data.size();
    if(!leaf)
    {
        test<concat>(res, value + data[depth],                          search, data, depth+1); // plus
        if (res) return; // early exit if the result has been found
        test<concat>(res, value * data[depth],                          search, data, depth+1); // mult
        if (concat)
        {
            if (res) return; // early exit if the result has been found
            test<concat>(res, concatenate(value, data[depth]),  search, data, depth+1); // concat
        }
    }
    else
        res |= (value == search);
}

template<bool concat>
bool test(const size_t search, const std::vector<size_t>& data)
{   
    bool res = false;
    test<concat>(res, data.front(), search, data, 1);
    return res;
}


template<bool concat>
void test(bool& p1, bool& p2, const size_t value, const size_t search, const std::vector<size_t>& data, const int depth)
{
    const bool leaf = depth == data.size();
    if(!leaf)
    {
        test<concat>(p1, p2, value + data[depth], search, data, depth+1); // plus
        if (res) return; // early exit if the result has been found
        test<concat>(p1, p2, value * data[depth], search, data, depth+1); // mult
        if (concat)
        {
            if (res) return; // early exit if the result has been found
            test<concat>(p1, p2, concatenate(value, data[depth]),  search, data, depth+1); // concat
        }
    }
    else
        res |= (value == search);
}

template<bool concat>
bool test(const size_t search, const std::vector<size_t>& data)
{   
    bool p1 = false, p2 = false;
    test<concat>(p1, p2, data.front(), search, data, 1);
    return res;
}

int main(int argc, char * argv[])
{   
    // read all data at once
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::string line;
    std::vector<size_t> data;
    size_t part1 = 0ul, part2 = 0ul;
    while(std::getline(file, line))
    {   
        data.clear();
        std::istringstream iss(line);
        size_t expected, a; char dummy; iss >> expected >> dummy;
        while(iss >> a) data.push_back(a);
        if (test<false>(expected, data)) part1 += expected;
        if (test<true>(expected, data)) part2 += expected;
    }

    const double t1 = t.micro().count(); t.reset();
    printf("Part 1: %zu\n", part1);
    printf("Part 2: %zu\n", part2);
    printf("Time: %f µs\n", t1);
    return 0;
}