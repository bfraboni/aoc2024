#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <unordered_map>
#include "../common/timer.h"

struct Solver
{
    mutable std::unordered_map<size_t, std::size_t> map;
    const std::vector<std::string>& atoms;
    const std::string& target;

    Solver(const std::vector<std::string>& atoms, const std::string& target) : atoms(atoms), target(target) {}

    // I know it is slow, and some other approaches could do better..
    inline std::size_t solve(const std::size_t pos = 0) const
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
                auto it = map.find(hash);
                if (it == map.end())
                {
                    const size_t nb = solve(pos + a.size());
                    it = map.emplace(hash, nb).first;
                }
                res += it->second;
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
            {
                // printf("atom: '%s'\n", atom.c_str());
                atoms.push_back(atom);
            }
        }
        else if (!line.empty())
        {
            designs.push_back(line);
            // printf("check: '%s' possible: %zu\n", line.c_str(), nb);
        }
    }

    const double t1 = t.micro().count(); t.reset();

    for (const auto& line: designs)
    {
        Solver s(atoms, line);
        const size_t nb = s.solve();
        part1 += (nb>0);
        part2 += nb;
    }

    const double t2 = t.micro().count(); t.reset();

    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu Time: %f µs\n", part1, t2);
    printf("Part 2: %zu Time: %f µs\n", part2, t2);
    return 0;
}