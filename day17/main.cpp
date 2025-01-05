#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include "../common/timer.h"

// AoC Day 17
// Part 1:
// Part 2:

// The adv instruction (opcode 0)
//     performs division. The numerator is the value in the A register. 
//     The denominator is found by raising 2 to the power of the instruction's combo operand. 
//     (So, an operand of 2 would divide A by 4 (2^2); an operand of 5 would divide A by 2^B.)
//     The result of the division operation is truncated to an integer and then written to the A register.
// The bxl instruction (opcode 1)
//     calculates the bitwise XOR of register B and the instruction's
//     literal operand, then stores the result in register B.
// The bst instruction (opcode 2)
//     calculates the value of its combo operand modulo 8
//     (thereby keeping only its lowest 3 bits), then writes that value to the B register.
// The jnz instruction (opcode 3)
//     does nothing if the A register is 0. However, if the A register is not zero,
//     it jumps by setting the instruction pointer to the value of its literal operand;
//     if this instruction jumps, the instruction pointer is not increased by 2 after this instruction.
// The bxc instruction (opcode 4)
//     calculates the bitwise XOR of register B and register C, then stores the result in register B.
//     (For legacy reasons, this instruction reads an operand but ignores it.)
// The out instruction (opcode 5)
//     calculates the value of its combo operand modulo 8, then outputs that value.
//     (If a program outputs multiple values, they are separated by commas.)
// The bdv instruction (opcode 6)
//     works exactly like the adv instruction except that the result is stored in the B register.
//     (The numerator is still read from the A register.)
// The cdv instruction (opcode 7)
//     works exactly like the adv instruction except that the result is stored in the C register.
//     (The numerator is still read from the A register.)

// enum Op {
//     adv= 0, // A = (int)div(A, 2^combo(D))
//     bxl= 1, // B = B XOR D
//     bst= 2, // B = combo(D) % 8
//     jnz= 3, // if A != 0: op = D
//     bxc= 4, // B = B XOR C
//     out= 5, // print(combo(D) % 8)
//     bdv= 6, // B = (int)div(A, 2^combo(D))
//     cdv= 7, // C = (int)div(A, 2^combo(D))
// };

// hand-unrolled program
inline int64_t run_single_iter(int64_t a, int64_t b = 0, int64_t c = 0)
{
    if (a == 0) return -1;
    b = a % 8;
    b = b ^ 2;
    c = a / (1 << b);
    b = b ^ 7;
    b = b ^ c;
    a = a / 8;
    return b % 8;
}

// the trick is the following:
// - each triplet of bits in A determine one digit of the output
// - the uppermost bits of A determines the last digit of the output
// - the lowermost bits of A determines the first digit of the output
// we thus starting from zero, reverse check every bit triplet and 
// look for matching program output.
int64_t search(const std::vector<int>& program, int id, int64_t a_tmp = 0)
{
    if (id < 0) return a_tmp;
    a_tmp = a_tmp << 3;                 // shift the temporary A value
    const int64_t target = program[id]; // target output digit
    for (int i = 0; i < 8; ++i)
    {
        const int64_t a_test = a_tmp + i;
        const int64_t digit = run_single_iter(a_test);
        if (digit == target)
        {
            // printf("\ntest %ld (%ld) target %d match %ld\n", a_test, curr, program[id], out);
            const int64_t a_found = search(program, id-1, a_test); // search for next digit
            if (a_found >= 0)
            {
                // printf("hit %ld (%d)\n", a_found, id);
                return a_found;
            }
        }
        // else
            // printf("test %d (%d) target %d fail %d\n", n, curr, program[id], out);
    }

    return -1;
}

int main(int argc, char * argv[])
{   
    // read all data at once
    Timer t;
    std::fstream file("input.txt", std::ios_base::in);
    std::string line;
    using calc_type = int; 
    std::vector<calc_type> program;
    calc_type a, b, c, d, op; char ch;
    size_t part1 = 0ul, part2 = 0ul;
    std::ostringstream oss;
    while(std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;
        else if (sscanf(line.c_str(), "Register A: %u", &a)) continue;
        else if (sscanf(line.c_str(), "Register B: %u", &b)) continue;
        else if (sscanf(line.c_str(), "Register C: %u", &c)) continue;
        else if (size_t pos = line.find("Program : "))
        {
            std::istringstream iss(line);
            iss.seekg(pos+10);
            program.clear();
            while(iss)
            {
                iss >> d >> ch;
                program.push_back(d);
            }

            // std::cout <<"line: " << line<< std::endl;
            // std::cout << "a: " << a << ", b: " << b<< ", c: " << c << ", program: '";
            // for(int i = 0; i<program.size(); ++i)
            //     std::cout << program[i] << " ";
            // std::cout << "'\n";

            // apply opcode
            auto eval = [&](const calc_type operand) -> calc_type
            {
                if      (operand == 0) return operand;
                else if (operand == 1) return operand;
                else if (operand == 2) return operand;
                else if (operand == 3) return operand;
                else if (operand == 4) return a;
                else if (operand == 5) return b;
                else if (operand == 6) return c;
                else
                {
                    printf("invalid operand ");
                    return 0u;
                }
            };

            const int sa = a, sb = b, sc = c;
            int id = 0;
            while(id < program.size())
            {
                const calc_type& op = program[id++];
                const calc_type& operand = program[id];
                if (op == 0)
                {
                    // std::cout << "\top: " << op;
                    // std::cout << ", operand: " << operand;
                    // std::cout << ", value: " << eval(operand);
                    // std::cout << ", a = a / 2^eval(d): " << a / (1 << eval(operand));
                    // std::cout << std::endl;
                    a = a / (1 << eval(operand));
                }
                else if (op == 1)
                {
                    // std::cout << "\top: " << op;
                    // std::cout << ", operand: " << operand;
                    // std::cout << ", value: " << operand;
                    // std::cout << ", b = b XOR eval(d): " << calc_type(b ^ operand);
                    // std::cout << std::endl;
                    b = b ^ operand;
                }
                else if (op == 2)
                {
                    // std::cout << "\top: " << op;
                    // std::cout << ", operand: " << operand;
                    // std::cout << ", value: " << eval(operand);
                    // std::cout << ", b = eval(d) % 8: " << eval(operand) % 8;
                    // std::cout << std::endl;
                    b = eval(operand) % 8;
                }
                else if (op == 3 && a != 0)
                {
                    id = operand;
                    continue;
                }
                else if (op == 4)
                {
                    // std::cout << "\top: " << op;
                    // std::cout << ", operand: " << operand;
                    // std::cout << ", value: " << eval(operand);
                    // std::cout << ", b = b XOR c: " << calc_type(b ^ c);
                    // std::cout << std::endl;
                    b = b ^ c;
                }
                else if (op == 5)
                {
                    // std::cout << "\top: " << op;
                    // std::cout << ", operand: " << operand;
                    // std::cout << ", value: " << eval(operand);
                    // std::cout << ", print: " << eval(operand) % 8;
                    // std::cout << std::endl;
                    const int val = eval(operand) % 8;
                    if (oss.tellp() != 0ul) oss << ",";
                    oss << val;
                }
                else if (op == 6)
                {
                    // std::cout << "\top: " << op;
                    // std::cout << ", operand: " << operand;
                    // std::cout << ", value: " << eval(operand);
                    // std::cout << ", b = a / 2^eval(d): " << a / (1 << eval(operand));
                    // std::cout << std::endl;
                    b = a / (1 << eval(operand));
                }
                else if (op == 7)
                {
                    // std::cout << "\top: " << op;
                    // std::cout << ", operand: " << operand;
                    // std::cout << ", value: " << eval(operand);
                    // std::cout << ", c = a / 2^eval(d): " << a / (1 << eval(operand));
                    // std::cout << std::endl;
                    c = a / (1 << eval(operand));
                }
                id++;
            }

            // std::cout << "a: " << a << ", b: " << b<< ", c: " << c << ", out: '" << oss.str() << "'\n\n";

            // std::cout << "\nprogram: '";
            // for(int i = 0; i<program.size(); ++i)
            //     std::cout << program[i] << " ";
            // std::cout << "'\n";
            const int64_t a2 = search(program, program.size()-1, 0);
            part2 = a2;
            // std::cout << "\na: " << a2 << "'\n\n";
        }
    }
    const double t1 = t.micro().count(); t.reset();
    printf("Time: %f µs\n", t1);
    printf("Part 1: %s\n", oss.str().c_str());
    printf("Part 2: %zu\n", part2);
    return 0;
}