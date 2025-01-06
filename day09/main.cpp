#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "../common/timer.h"

// AoC Day 9
// Part 1: Compress data, take the leftmost blocks and push them back in the front empty space.
// Part 2:

inline size_t solve1(std::vector<int>& data)
{
    size_t res = 0ul;
    for (size_t lid = 0, rid = data.size()-1; lid <= rid; lid++)
    {
        if (data[lid] < 0) // there is an empty space to fill
        {
            while(data[rid] < 0 && rid > lid) rid--; // find first non empty cell
            std::swap(data[lid], data[rid]); // swap values
        }
        if (data[lid] >= 0) // if the space is filled
            res += data[lid] * lid;
        else
            break;
    }
    return res;
}

int main(int argc, char * argv[])
{   
    // read all data at once
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::vector<int> data;
    char c; int count = 0;
    struct Block { int start, end, size = 0, used = 0; };
    std::vector<Block> full, empty; 
    while(file >> c)
    {
        const bool nonempty = count%2 == 0;
        const int n = c-'0';
        const int id = nonempty ? count/2 : -n; // encode empty space with negative values
        const Block b = {(int)data.size(), (int)data.size()+n, n, 0};
        if (nonempty)   full.emplace_back(b);
        else            empty.emplace_back(b);
        data.insert(data.end(), n, id);
        count++;
    }
    std::reverse(full.begin(), full.end());
    const double t1 = t.micro().count(); t.reset();

    // part 1
    std::vector<int> copy = data;
    const size_t part1 = solve1(copy);
    const double t2 = t.micro().count(); t.reset();

    // part 2
    // std::swap(data, copy);
    // std::vector<Block> empty, full;
    // int rid = data.size() - 1;
    // while (rid >= 0)
    // {
    //     if (data[rid] < 0)
    //     {
    //         --rid;
    //     }
    //     else
    //     {
    //         Block b;
    //         b.end = rid+1;
    //         while(rid >= 0 && data[rid] == data[b.end-1]) rid--;
    //         b.start = rid+1;
    //         b.size = b.end - b.start;
    //         b.used = false;
    //         full.push_back(b);
    //         printf("data block (%d, %d) of size: %d\n", b.start, b.end, b.size);
    //         const Block& other = full2[full2.size()-(full.size())];
    //         printf("other block (%d, %d) of size: %d\n", other.start, other.end, other.size);
    //     }
    // }

    // int lid = 0;
    // while (lid < data.size())
    // {
    //     if (data[lid] >= 0)
    //     {
    //         ++lid;
    //     }
    //     else
    //     {
    //         Block b;
    //         b.start = lid;
    //         while(lid < data.size() && data[lid] < 0) lid++;
    //         b.end = lid;
    //         b.size = b.end - b.start;
    //         b.used = false;
    //         empty.push_back(b);
    //         // printf("empty block (%d, %d) of size: %d\n", b.start, b.end, b.size);
    //     }
    // }
    
    // auto print_data = [](const std::vector<int>& data)
    // {
    //     for(auto i : data)
    //     {
    //         if (i >= 0 && i <= 9) printf("%d", i);
    //         else if (i >= 0) printf("B");
    //         else printf(".");
    //     }
    //     printf("\n");
    // };
    // auto print_move = [&](const std::vector<int>& data, const Block& src, const Block& dst)
    // {
    //     // print_data(data);
    //     for(int i = 0; i < data.size(); ++i)
    //     {
    //         if (i >= src.start && i < src.end) printf("^");
    //         else if (i >= dst.start && i < dst.end) printf("#");
    //         else printf(" ");
    //     }
    //     printf("\n\n");
    // };

    // for each data block
    for (auto& f : full)
    // const int start = full2.size()-1;
    // for (int j = start; j >= 0; --j)
    {
        // const auto& f = full2[j];

    #if 0
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
                    printf("full block (%d, %d) of size: %d\n", f.start, f.end, f.size);
                    printf("wants to swap with block (%d, %d) of size: %d\n", e.start, e.end, e.size);
                    // print_move(data, f, e);
                    for(int i = 0; i < f.size; ++i)
                        std::swap(data[f.start+i], data[e.start+i]);
                    break;
                }
            }
        }

        for (auto& e2 : empty2)
        {
            if (/*e2.used ||*/ e2.size >= f.size && e2.end < f.start)
            {
                printf("which should match block (%d, %d) of size: %d\n", e2.start, e2.end, e2.size);
                // if (e2.size != e.size) exit(1);
                e2.start += f.size;
                e2.size = e2.end - e2.start;
                break;
            }
        }
    #else
        // int id = 0;
        // while (id < f.start)
        // {
        //     if (data[id] >= 0) id++;
        //     else
        //     {
        //         Block e;
        //         e.start = id;
        //         while(id < f.start && data[id] < 0) id++;
        //         e.end = id;
        //         e.size = e.end-e.start;

        //         if (e.size >= f.size)
        //         {
        //             printf("full block (%d, %d) of size: %d\n", f.start, f.end, f.size);
        //             printf("wants to swap with block (%d, %d) of size: %d\n", e.start, e.end, e.size);
        //             printf("from ");
        //             for(int i = 0; i < f.size; ++i)
        //                 printf("%d ", data[f.start+i]);
        //             printf(" to ");
        //             for(int i = 0; i < f.size; ++i)
        //                 printf("%d ", data[e.start+i]);
        //             printf("\n");
        //             break;
        //         }
        //     }
        // }
        // find any empty block that'd hold that data V1
        for (auto& e : empty)
        {
            if (e.size < f.size || e.end > f.start) continue;
            // printf("which should match block (%d, %d) of size: %d\n", e.start, e.end, e.size);
            // printf("from ");
            // for(int i = 0; i < f.size; ++i)
            //     printf("%d ", data[f.start+i]);
            // printf(" to ");
            // for(int i = 0; i < f.size; ++i)
            //     printf("%d ", data[e.start+i]);
            // printf("\n");
            // print_move(data, f, e);
            for(int i = 0; i < f.size; ++i)
                std::swap(data[f.start+i], data[e.start+i]);

            e.start += f.size;
            e.size = e.end - e.start;
            // if (e.size == 0)
                // e.suze
            // e.used = e.size == 0;
            break;
        }
        // f.used = true;

        // find any empty block that'd hold that data V2
    #endif
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
    const double t3 = t.micro().count(); t.reset();
    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu, time: %f µs\n", part1, t2);
    printf("Part 2: %zu, time: %f µs\n", part2, t3);
    return 0;
}