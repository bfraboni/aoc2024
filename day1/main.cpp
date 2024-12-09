#include <vector>
#include <fstream>
#include <cassert>
#include <algorithm>
#include "../common/timer.h"

//! AoC Day 1
//! Part 1: find the sum of difference between the sorted pairs of numbers.
//! Part 2: Calculate a similarity score by adding up each number in the left list
//!         multiplied by the number of times that number appears in the right list.

inline size_t solve1(std::vector<int>& left, std::vector<int>& right)
{
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    const size_t size = left.size();

    size_t sum = 0ul;
    for(size_t i = 0; i < size; ++i)
        sum += std::abs(left[i] - right[i]);

    return sum;
}

//! NOTE: this only works if the lists are sorted beforehand
inline size_t solve2(const std::vector<int>& left, const std::vector<int>& right)
{   
    const size_t size = left.size();
    size_t similarity = 0ul, idl = 0ul, idr = 0ul;
    while(idl < size && idr < size)
    {
        if      (left[idl] < right[idr]) {++idl;} // left is lower than right
        else if (right[idr] < left[idl]) {++idr;} // right is lower than left
        else    {similarity += left[idl]; ++idr;} // we found a match, add one occurrence to the similarity
    }
    return similarity;
}

int main(int argc, char * argv[])
{
    // load data
    Timer t;
	std::fstream file("input.txt", std::ios_base::in);
    int i, count = 0;
    std::vector<int> left, right;
    while(file >> i)
    {   
        std::vector<int>& vec = (++count%2) ? left : right; 
        vec.push_back(i);
    }
    assert(left.size() == right.size());

    // solve
    const double t1 = t.micro().count(); t.reset();
    const size_t distance = solve1(left, right);
    const double t2 = t.micro().count(); t.reset();
    const size_t similarity = solve2(left, right);
    const double t3 = t.micro().count();

    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu, Time: %f µs\n", distance, t2);
    printf("Part 2: %zu, Time: %f µs\n", similarity, t3);

    return 0;
}