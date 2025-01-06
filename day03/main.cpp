#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../common/timer.h"

// AoC Day 3: https://adventofcode.com/2024/day/3
// Part 1: Add up all of the results of the multiplications fo the form 'mul(X,Y)'.
// Part 2: Now take into account 'do()' and 'don't()' instructions that enable/disable the following operations.

enum Part : bool { One = 0, Two = 1 };
template<Part part>
inline size_t solve()
{
    // read all file at once
    std::fstream file("input.txt", std::ios_base::in);
    std::stringstream iss;
    iss << file.rdbuf();
    std::string line = iss.str();
    size_t res = 0ul, start = 0ul, stop;
    while(true)
    {
        stop = part == One ? std::string::npos : line.find("don't()", start);
        while(true)
        {
            std::size_t pos = line.find("mul(", start);
            if (pos >= stop) break;

            start = pos+4;
            iss.seekg(start);
            auto parse = [&](int& value, const char delimiter){
                //! NOTE: with my input the following condition can be skipped,
                //!       but not sure that'd work for other inputs. 
                if (!std::isdigit(line[iss.tellg()])) return false;
                char q; iss >> value >> q;
                return q == delimiter; 
            };

            int v1 = 0, v2 = 0;
            if (parse(v1, ',') && parse(v2, ')'))
                res += v1 * v2;
        }
        if (stop == std::string::npos) break;
        start = line.find("do()", start+7);
        if (start == std::string::npos) break;
        start += 4;
    }

    return res;
}

int main(int argc, char * argv[])
{
    Timer t;
    const size_t part1 = solve<One>();
    const double t1 = t.micro().count(); t.reset();
    const size_t part2 = solve<Two>();
    const double t2 = t.micro().count(); t.reset();

    printf("Part 1: %zu Time: %f µs\nPart 2: %zu Time: %f µs\n", part1, t1, part2, t2);
    return 0;
}