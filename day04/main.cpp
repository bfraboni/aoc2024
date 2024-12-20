#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../common/timer.h"

// AoC Day 4
// Part 1: Find all the occurrence of the word XMAS in the grid in any of the 8-star directions (N,E,S,W,NE,NW,SE,SW).
// Part 2: Find all the X shaped MAS words. 

template<int di, int dj>
bool check(const std::vector<std::string>& data, const int i, const int j, const int w, const int h)
{
    return i+3*di >= 0 && i+3*di < h && j+3*dj >= 0 && j+3*dj < w
        && data[i+1*di][j+1*dj] == 'M' && data[i+2*di][j+2*dj] == 'A' && data[i+3*di][j+3*dj] == 'S';
}

int main(int argc, char * argv[])
{   
    // read all data at once
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::vector<std::string> data;
    std::string line;
    while(std::getline(file, line))
        data.push_back(line);

    // solve puzzles
    const double t1 = t.micro().count(); t.reset();
    const size_t h = data.size();
    const size_t w = data[0].size();
    size_t part1 = 0ul, part2 = 0ul;
    for (int i = 0; i < h; ++i)
    for (int j = 0; j < w; ++j)
    {
        // if char is an X, check in the neighborhood for XMAS words
        if (data[i][j] == 'X')
        {
            // test all eight directions
            part1 += check< 0, 1>(data, i, j, w, h);
            part1 += check< 1, 1>(data, i, j, w, h);
            part1 += check< 1, 0>(data, i, j, w, h);
            part1 += check< 1,-1>(data, i, j, w, h);
            part1 += check< 0,-1>(data, i, j, w, h);
            part1 += check<-1,-1>(data, i, j, w, h);
            part1 += check<-1, 0>(data, i, j, w, h);
            part1 += check<-1, 1>(data, i, j, w, h);
        }
        else if (data[i][j] == 'A')
        {
            // test both diagonals
            if (i+1 < h && j+1 < w && i-1 >= 0 && j-1 >= 0)
            {
                const bool A = data[i+1][j+1] == 'M' && data[i-1][j-1] == 'S';
                const bool B = data[i+1][j+1] == 'S' && data[i-1][j-1] == 'M';
                const bool C = data[i+1][j-1] == 'M' && data[i-1][j+1] == 'S';
                const bool D = data[i+1][j-1] == 'S' && data[i-1][j+1] == 'M';
                part2 += (A || B) && (C || D);
            }
        }
    }

    const double t2 = t.micro().count(); t.reset();
    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu\n", part1);
    printf("Part 2: %zu\n", part2);
    printf("Time: %f µs\n", t2);
    return 0;
}