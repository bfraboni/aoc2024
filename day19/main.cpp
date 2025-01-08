#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <unordered_map>
#include "../common/timer.h"

struct Solver
{
    std::vector<size_t> map;
    const std::vector<std::string>& atoms;
    const std::string& target;

    Solver(const std::vector<std::string>& atoms, const std::string& target)
    : atoms(atoms), target(target), map(atoms.size() * target.size(), -1) {}

    //! TODO: I know it is slow, and some other approaches could do better, i.e. with a trie.
    inline std::size_t solve(const std::size_t pos = 0)
    {
        if (pos == target.size()) return 1;
        std::size_t res = 0;
        for (int i = 0; i < atoms.size(); i++)
        {
            const auto& a = atoms[i];
            const std::string_view view(target.data() + pos, a.size());
            if (a == view)
            {
                const size_t hash = i * target.size() + pos;
                if (map[hash] == size_t(-1))
                    map[hash] = solve(pos + a.size());
                res += map[hash];
            }
        }
        return res;
    }
};

int main(int argc, char * argv[])
{   
    // read all data at once
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::string line, atom;
    std::vector<std::string> atoms, designs;
    size_t part1 = 0ul, part2 = 0ul;
    while(std::getline(file, line))
    {
        if (line.find(',') != std::string::npos)
        {
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end()); // remove spaces
            std::istringstream iss(line);
            while(std::getline(iss, atom, ','))
                atoms.push_back(atom);
        }
        else if (!line.empty())
            designs.push_back(line);
    }

    const double t1 = t.micro().count(); t.reset();

    for (const auto& line: designs)
    {
        Solver s(atoms, line);
        const size_t nb = s.solve();
        part1 += (nb > 0);
        part2 += nb;
    }

    const double t2 = t.micro().count(); t.reset();

    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu Time: %f µs\n", part1, t2);
    printf("Part 2: %zu Time: %f µs\n", part2, t2);
    return 0;
}