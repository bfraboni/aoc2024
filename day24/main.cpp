#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <queue>
#include <set>
#include <unordered_map>
#include <queue>
#include "../common/timer.h"

// AoC Day 24
// Part 1:
// Part 2:

enum Op : int8_t { AND = 0, OR, XOR };
inline std::string to_string(Op op) { return op == OR ? " or  " : (op == AND ? " and " : " xor "); }

struct Command
{
    std::string stra, strb, stro;
    int8_t op;
    int id;
    int ia, ib, io;
};

// https://en.wikipedia.org/wiki/Adder_(electronics)#/media/File:Halfadder.gif
struct HalfAdder
{
    bool a, b;
    HalfAdder(bool a, bool b) : a(a), b(b) {}
    inline bool out()      const { return a ^ b; }
    inline bool carry()    const { return a & b; }
};

// https://en.wikipedia.org/wiki/Adder_(electronics)#/media/File:Fulladder.gif
struct FullAdder
{
    bool a, b, c;
    FullAdder() : a(false), b(false), c(false) {}
    FullAdder(bool a, bool b, bool c) : a(a), b(b), c(c) {}
    inline void init(bool a, bool b, bool c) { this->a = a; this->b = b; this->c = c; }  
    inline bool out()      const { return (a ^ b) ^ c; }
    inline bool carry()    const { return (a & b) | (c & (a ^ b)); }
};

std::vector<Command> apply(std::queue<Command>& commands, std::unordered_map<std::string, bool>& map)
{
    constexpr bool verbose = false;
    std::vector<Command> ordered;
    while(!commands.empty())
    {
        const Command& c = commands.front();
        const bool valid = map.find(c.stra) != map.end() && map.find(c.strb) != map.end();
        if (valid)
        {
            // eval the operation and store result
            if      (c.op == OR)    map[c.stro] = map[c.stra] | map[c.strb];
            else if (c.op == AND)   map[c.stro] = map[c.stra] & map[c.strb];
            else if (c.op == XOR)   map[c.stro] = map[c.stra] ^ map[c.strb];
            ordered.push_back(c);
            if constexpr (verbose)
                std::cout << "\t" << c.stro << " = " << c.stra << to_string((Op)c.op) << c.strb << std::endl;
        }
        else
        {
            // push command to the back
            commands.push(commands.front());
        }
        commands.pop();
    }
    return ordered;
}

std::string to_string(int id)
{
    std::string out(3, ' ');
    out[2] = char(id >> 16);    id -= int(out[2]) << 16;
    out[1] = char(id >> 8);     id -= int(out[1]) << 8;
    out[0] = char(id);
    return out;
}
int to_int(const std::string& str) { return int(str[0]) + (int(str[1]) << 8) + (int(str[2]) << 16); }
int to_int(const char str[]) { return int(str[0]) + (int(str[1]) << 8) + (int(str[2]) << 16); }

int main(int argc, char * argv[])
{
    constexpr bool verbose = false;
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::unordered_map<std::string, bool> map;
    std::set<std::string> xs, ys, zs;
    std::string line;
    std::queue<Command> commands;
    char a[4]; int value;
    char op[4], b[4], out[4];
    int id = 0;
    while(std::getline(file, line))
    {
        if (sscanf(line.c_str(), "%3s: %d", a, &value) == 2) // init
        {
            std::string stra(a);
            map[stra] = bool(value);
            if (stra[0] == 'x') xs.insert(stra);
            if (stra[0] == 'y') ys.insert(stra);
        }
        else if (sscanf(line.c_str(), "%3s %3s %3s -> %3s", a, op, b, out) == 4) // operations
        {
            const std::string stra(a), strb(b), stro(out);
            const std::string& first = stra > strb ? strb : stra, second = stra > strb ? stra : strb;
            if (op[0] == 'O')       commands.push({first, second, stro, OR, id++ });
            else if (op[0] == 'A')  commands.push({first, second, stro, AND, id++ });
            else if (op[0] == 'X')  commands.push({first, second, stro, XOR, id++ });
            if (stro[0] == 'z') zs.insert(stro);


            if constexpr (verbose)
            {
                std::cout << "stra: " << stra << std::endl;
                std::cout << "int stra: "<< to_int(stra) << std::endl;
                std::cout << "int a: "<< to_int(a) << std::endl;
                std::cout << "string int stra: "<< to_string(to_int(stra)) << std::endl;
                std::cout << "string int a: "<< to_string(to_int(a)) << std::endl;
            }
        }
    }

    // part1, fast version after abandoning the O(N^2) over edges one
    const double t1 = t.micro().count(); t.reset();
    std::vector<Command> ordered = apply(commands, map);

    // for all bits of X and Y do the operation by hand
    std::vector<std::string> vx(xs.begin(), xs.end());
    std::vector<std::string> vy(ys.begin(), ys.end());
    std::vector<std::string> vz(zs.begin(), zs.end());
    FullAdder add;
    int count = 0;
    std::set<std::string> fixed;
    for (int i = 0; i < vx.size(); ++i)
    {
        const bool cin = add.carry();
        add.init(map[vx[i]], map[vy[i]], cin);
        if constexpr (verbose)
        {
            printf("ci %d x %d y %d s %d co %d : z %d : %s\n",
                (int) cin
                ,(int) map[vx[i]]
                ,(int) map[vy[i]]
                ,(int) add.out()
                ,(int) add.carry()
                ,(int) map[vz[i]]
                , (map[vz[i]] == add.out() ? " correct" : " incorrect")
            );
        }

        // gather all operations related to the current output bit
        std::vector<Command> fulladd;
        int last = 0;
        for (const auto& c : ordered)
        {
            if (c.stra == vx[i] || c.strb == vx[i] || c.stra == vy[i] || c.strb == vy[i])
            {
                if constexpr (verbose)
                    std::cout << "\t" << c.stro << " = " << c.stra << to_string((Op)c.op) << c.strb << std::endl;
                fulladd.push_back(c);

                if (i > 0) // half adder for bit 1
                {
                    for (const auto& c2 : ordered)
                    {
                        if (c2.stra == c.stro || c2.strb == c.stro)
                        {
                            if constexpr (verbose)
                                std::cout << "\t" << c2.stro << " = " << c2.stra << to_string((Op)c2.op) << c2.strb << std::endl;
                            fulladd.push_back(c2);
                            if (c2.stro[0] == 'z') last = fulladd.size()-1;
                        }
                    }
                }
            }
        }

        for (const auto& c : fulladd)
            map.erase(c.stro); // reset outputs

        // if (map[vz[i]] != add.out())
        {
            // validation
            if constexpr (verbose)
                printf("\t--- correction ---\n");
            std::string swapped;
            if (fulladd[last].op != XOR) // the output wire is wrong
            {
                // we need to swap this wire and one of the other XOR
                for (int j = 0; j < fulladd.size(); ++j)
                {
                    auto& c = fulladd[last];
                    auto& c2 = fulladd[j];
                    if(j==last) continue;
                    else if (c2.op == XOR && (c2.stra[0] < 'x' || c2.stra[0] > 'y'))
                    {
                        swapped = c2.stro;
                        std::swap(c.stro, c2.stro);
                        if constexpr (verbose)
                        {
                            std::cout << "\t" << count++ << ": " << c2.stro << " = " << c2.stra << to_string((Op)c2.op) << c2.strb << std::endl;
                            std::cout << "\t" << count++ << ": " << c.stro << " = " << c.stra << to_string((Op)c.op) << c.strb << std::endl;
                        }
                        fixed.insert(c2.stro);
                        fixed.insert(c.stro);
                        break;
                    }
                }
            }
            else if (map.find(fulladd[last].stra) == map.end())
            {
                // the input wire is wrong
                const auto& c = fulladd[last];
                bool wrong = false; int inputxor = -1, wrongid = -1;
                for (int j = 0; j < fulladd.size(); ++j)
                {
                    auto& c2 = fulladd[j];
                    // std::cout << "\t" << count++ << ": " << c2.stro << " = " << c2.stra << to_string((Op)c2.op) << c2.strb << std::endl;
                    if(j==last) continue;
                    else if (c2.stro == c.stra && c2.op != XOR)
                    {
                        if constexpr (verbose)
                            printf("\twrong input wire , id: %d\n", j);
                        wrong = true;
                        wrongid = j;
                    }
                    else if (c2.op == XOR && (c2.stra[0] >= 'x' || c2.stra[0] <= 'y'))
                    {
                        // printf("\tinput wire , id: %d\n", j);
                        inputxor = j;
                    }
                }

                if (wrong)
                {
                    auto& c = fulladd[wrongid];
                    auto& c2 = fulladd[inputxor];
                    std::swap(c.stro, c2.stro);
                    if constexpr (verbose)
                    {
                        std::cout << "\t" << count++ << ": " << c2.stro << " = " << c2.stra << to_string((Op)c2.op) << c2.strb << std::endl;
                        std::cout << "\t" << count++ << ": " << c.stro << " = " << c.stra << to_string((Op)c.op) << c.strb << std::endl;
                    }
                    fixed.insert(c2.stro);
                    fixed.insert(c.stro);
                }
            }
            else if (map.find(fulladd[last].strb) == map.end())  // the input wire is wrong
            {
                // the input wire is wrong
                const auto& c = fulladd[last];
                bool wrong = false; int inputxor = -1, wrongid = -1;
                for (int j = 0; j < fulladd.size(); ++j)
                {
                    auto& c2 = fulladd[j];
                    // std::cout << "\t" << count++ << ": " << c2.stro << " = " << c2.stra << to_string((Op)c2.op) << c2.strb << std::endl;
                    if(j==last) continue;
                    else if (c2.stro == c.strb && c2.op != XOR)
                    {
                        if constexpr (verbose)
                            printf("\twrong input wire , id: %d\n", j);
                        wrong = true;
                        wrongid = j;
                    }
                    else if (c2.op == XOR && (c2.stra[0] >= 'x' || c2.stra[0] <= 'y'))
                    {
                        // printf("\tinput wire , id: %d\n", j);
                        inputxor = j;
                    }
                }

                if (wrong)
                {
                    auto& c = fulladd[wrongid];
                    auto& c2 = fulladd[inputxor];
                    std::swap(c.stro, c2.stro);
                    if constexpr (verbose)
                    {
                        std::cout << "\t" << count++ << ": " << c2.stro << " = " << c2.stra << to_string((Op)c2.op) << c2.strb << std::endl;
                        std::cout << "\t" << count++ << ": " << c.stro << " = " << c.stra << to_string((Op)c.op) << c.strb << std::endl;
                    }
                    fixed.insert(c2.stro);
                    fixed.insert(c.stro);
                }
            }

            if (i>0 && fulladd.size()<5)
            {
                if constexpr (verbose)
                {
                    printf("\t--- missing ---\n");
                    printf("\tswapped %s\n", swapped.c_str());
                }
                // find the missing command
                for (const auto& c : ordered)
                {
                    if (c.stra == swapped || c.strb == swapped)
                    {
                        if constexpr (verbose)
                            std::cout << "\t" << c.stro << " = " << c.stra << to_string((Op)c.op) << c.strb << std::endl;
                        fulladd.push_back(c);
                        map.erase(c.stro);
                    }
                }
            }

            for (const auto& c : fulladd)
            {
                commands.push(c);
                // map.erase(c.stro); // reset value :)
            }
            fulladd.clear();
            fulladd = apply(commands, map);
            {
                if constexpr (verbose)
                    printf("\t--- fixed ---\n");
                add.init(map[vx[i]], map[vy[i]], cin);

                if constexpr (verbose)
                {
                    printf("\tci %d x %d y %d s %d co %d : z %d : %s\n",
                        (int) cin
                        ,(int) map[vx[i]]
                        ,(int) map[vy[i]]
                        ,(int) add.out()
                        ,(int) add.carry()
                        ,(int) map[vz[i]]
                        , (map[vz[i]] == add.out() ? " correct" : " incorrect")
                    );
                }
            }

            if (map[vz[i]] != add.out())
            {
                printf("error, the bits mismatch\n");
                exit(1);
            }
        }
    }
    const double t2 = t.micro().count(); t.reset();

    auto convert = [&](const std::set<std::string>& inputs) -> size_t
    {
        size_t res = 0ul, shift = 0ul;
        for (const auto& str : inputs)
        {
            res += size_t(map[str]) << shift;
            shift++;
        }
        return res;
    };
    const size_t Z = convert(zs);

    if constexpr (verbose)
    {
        const size_t X = convert(xs);
        const size_t Y = convert(ys);
        printf("X bits %zu\n", xs.size());
        printf("Y bits %zu\n", ys.size());
        printf("Z bits %zu\n", zs.size());
        printf("%zu + %zu = %zu ; %zu\n", X, Y, X+Y, Z);
    }

    // hell yeah, finally, ugliest entry so far, but it works !
    std::string str;
    for (const auto& s:fixed)
        str += s + ',';

    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu, Time: %f µs\n", Z, t1);
    printf("Part 2: %s, Time: %f µs\n", str.c_str(), t2);
}