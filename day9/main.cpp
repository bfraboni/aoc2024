#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "../common/timer.h"

// AoC Day 9
// Part 1: Compress data, take the leftmost blocks and push them back in the front empty space.
// Part 2:

int main(int argc, char * argv[])
{   
    // read all data at once
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::vector<int> data;
    data.reserve(100000);
    char c; int count = 0;
    while(file >> c)
    {
        const bool full = count%2 == 0;
        const int n = c-'0';
        const int id = full ? count/2 : -n;
        data.insert(data.end(), n, id);
        count++;
    }
    // printf("numbers: %d\n", count);
    // for(auto i : data)
    // {
    //     if (i >= 0 && i <= 9) printf("%d", i);
    //     else if (i >= 0) printf("B");
    //     else printf(".");
    // }
    // printf("\n");
    std::vector<int> copy = data; // for part #2

    // part 1
    size_t part1 = 0UL;
    {
        size_t lid = 0, rid = data.size()-1;
        while (lid <= rid)
        {
            if (data[lid] < 0) // there is an empty space to fill
            {
                while(data[rid] < 0 && rid > lid) rid--; // find first non empty cell
                std::swap(data[lid], data[rid]); // swap values
            }
            if (data[lid] >= 0) // if the space is filled
            {
                part1 += data[lid] * lid;
                // printf("lid: %zu data[lid]: %d\n", lid, data[lid]);
            }
            else
                break;

            lid++;
        }
    }

    // part 2
    std::swap(data, copy);
    struct Block { int start, end, size = 0, used = 0; };
    std::vector<Block> empty, full;
    int rid = data.size() - 1;
    while (rid >= 0)
    {
        if (data[rid] < 0)
        {
            --rid;
        }
        else
        {
            Block b;
            b.end = rid+1;
            while(rid >= 0 && data[rid] == data[b.end-1]) rid--;
            b.start = rid+1;
            b.size = b.end - b.start;
            b.used = false;
            full.push_back(b);
            // printf("data block (%d, %d) of size: %d\n", b.start, b.end, b.size);
        }
    }

    int lid = 0;
    while (lid < data.size())
    {
        if (data[lid] >= 0)
        {
            ++lid;
        }
        else
        {
            Block b;
            b.start = lid;
            while(lid < data.size() && data[lid] < 0) lid++;
            b.end = lid;
            b.size = b.end - b.start;
            b.used = false;
            empty.push_back(b);
            // printf("empty block (%d, %d) of size: %d\n", b.start, b.end, b.size);
        }
    }
    
    auto print_data = [](const std::vector<int>& data)
    {
        for(auto i : data)
        {
            if (i >= 0 && i <= 9) printf("%d", i);
            else if (i >= 0) printf("B");
            else printf(".");
        }
        printf("\n");
    };
    auto print_move = [&](const std::vector<int>& data, const Block& src, const Block& dst)
    {
        print_data(data);
        for(int i = 0; i < data.size(); ++i)
        {
            if (i >= src.start && i < src.end) printf("^");
            else if (i >= dst.start && i < dst.end) printf("#");
            else printf(" ");
        }
        printf("\n\n");
    };

    // for each data block
    for (auto& f : full)
    {
        // find any empty block that'd hold that data V1
        // for (auto& e : empty)
        // {
        //     if (e.used || e.size < f.size) continue;
        //     // print_move(data, f, e);
        //     for(int i = 0; i < f.size; ++i)
        //         std::swap(data[f.start+i], data[e.start+i]);

        //     e.start += f.size;
        //     e.size = e.end - e.start;
        //     e.used = e.size == 0;
        //     break;
        // }
        // f.used = true;

        // find any empty block that'd hold that data V2
        int id = 0;
        while (id < f.start)
        {
            if (data[id] >= 0) id++;
            else
            {
                Block e;
                e.start = id;
                while(id < f.start && data[id] < 0) id++;
                e.end = id;
                e.size = e.end-e.start;
                if (e.size >= f.size)
                {
                    // print_move(data, f, e);
                    for(int i = 0; i < f.size; ++i)
                        std::swap(data[f.start+i], data[e.start+i]);
                    break;
                }
            }
        }

    }

    size_t id = 0, part2 = 0ul;
    for(auto i : data)
    {
        // if (i >= 0 && i <= 9) printf("%d", i);
        // else if (i >= 0) printf("B");
        // else printf(".");
        if (i >= 0) part2 += i * id;
        id++;
    }
    // printf("\n");

    // for(auto i : data)
    //     if (i >= 0) printf("B");
    //     else printf(".");
    // printf("\n");

    // for(const auto& i : data)
    // {
    //     if (i == -1) printf(".");
    //     else printf("%d", i);
    // }
    const double t1 = t.micro().count(); t.reset();
    printf("Part 1: %zu\n", part1);
    printf("Part 2: %zu\n", part2);
    printf("Time: %f µs\n", t1);
    return 0;
}