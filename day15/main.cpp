#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include "../common/timer.h"

// AoC Day 15: https://adventofcode.com/2024/day/15
// Part 1: Move the robot "@" and push the boxes "O" along his path if possible. Compute the sum of box indices.
// Part 2: Update the map such that it is twice a wide, but the moves are still of size one.
//         Move the robot through that new map. Compute the sum of box indices.

enum Dir { Top = 0, Bottom = 1, Left = 2, Right = 3 };

struct Vec2
{
    int x, y;
    inline Vec2() {}
    inline Vec2(int x, int y) : x(x), y(y) {}
    inline Vec2(int id, int w, int h) : x(id % w), y(id / w) {}
    inline int index(const int w, const int h) const { return y * w + x; }
    inline bool valid(const int w, const int h) const { return x >= 0 && x < w && y >= 0 && y < h; }
    inline void print() const { std::cout << "(" << x << " " << y << ")"; }
    inline Vec2 operator+(const Vec2& other) const { return Vec2(x+other.x, y+other.y); }
    inline Vec2 operator-(const Vec2& other) const { return Vec2(x-other.x, y-other.y); }

    inline Vec2 next(const char& c) const {
        if (c == '^')       return Vec2(x, y-1);
        else if (c == 'v')  return Vec2(x, y+1);
        else if (c == '>')  return Vec2(x+1, y);
        else                return Vec2(x-1, y);
    }

    template<Dir d>
    inline Vec2 next() const {
        if constexpr      (d == Top)    return Vec2(x, y-1);
        else if constexpr (d == Bottom) return Vec2(x, y+1);
        else if constexpr (d == Left)   return Vec2(x+1, y);
        else                            return Vec2(x-1, y);
    }
};

inline bool push(std::vector<std::string>& map, const Vec2& pos, const char& dir)
{
    if      (map[pos.y][pos.x] == '.') return true;
    else if (map[pos.y][pos.x] == 'O')
    {
        const Vec2 next = pos.next(dir);
        if (push(map, next, dir))
        {
            std::swap(map[pos.y][pos.x], map[next.y][next.x]);
            return true;
        }
    }
    return false;
}

// there is a bit of trickery here.. not sure I need all of that
struct Part2
{
    std::vector<std::string>& map;
    const int width;
    const int height;
    std::queue<int> q;
    std::unordered_set<int> s;

    Part2(std::vector<std::string>& map) : map(map), width(map[0].size()), height(map.size()) {}

    inline bool compare(const Vec2& pos, const char& c) const { return map[pos.y][pos.x] == c; }

    inline bool move(const Vec2& pos, const char dir)
    {
        s.clear();
        bool push = false;
        switch (dir)
        {
            case '^': push = pushable<Top>(pos);    break;
            case 'v': push = pushable<Bottom>(pos); break;
            case '>': push = pushable<Left>(pos);   break;
            case '<': push = pushable<Right>(pos);  break;
        }
        if (push)
        {
            // apply push chain
            while (!q.empty())
            {
                // move box
                const Vec2 left(q.front(), width, height), nextleft = left.next(dir);
                map[left.y][left.x] = map[left.y][left.x+1] = '.';    // reset
                map[nextleft.y][nextleft.x  ] = '[';                  // move box left
                map[nextleft.y][nextleft.x+1] = ']';                  // move box right
                q.pop();
            }
            map[pos.y][pos.x] = '.';
            return true;
        }
        else
            while (!q.empty()) q.pop();
        return false;
    }

    template<Dir d>
    inline bool pushable(const Vec2& pos/*, const char dir*/)
    {
        if      (compare(pos, '.')) return true;
        else if (compare(pos, '#')) return false;
        else if (compare(pos, '[') | compare(pos, ']'))
        {
            const Vec2& left = compare(pos, '[') ? pos : pos.next<Right>();
            const Vec2 right(left.x+1, left.y), nextleft = left.next<d>(), nextright = right.next<d>();
            bool test = false;
            if      constexpr (d == Left)   test = pushable<d>(nextright);
            else if constexpr (d == Right)  test = pushable<d>(nextleft);
            else    test = pushable<d>(nextleft) & pushable<d>(nextright);
            if (test)
            {
                const int id = left.index(width, height);
                if (s.insert(id).second) q.push(id);
                return true;
            }
        }
        return false;
    }
};

int main(int argc, char * argv[])
{   
    // read all data at once
    Timer t;
#if 1
    std::fstream file("input.txt", std::ios_base::in);
#else
    // std::string filedata = "##########\n#..O..O.O#\n#......O.#\n#.OO..O.O#\n#..O@..O.#\n#O#..O...#\n#O..O..O.#\n#.OO.O.OO#\n#....O...#\n##########\n\n<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^\nvvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v\n><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<\n<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^\n^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><\n^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^\n>^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^\n<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>\n^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>\nv^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^\n";
    // std::string filedata = "#######\n#...#.#\n#.....#\n#..OO@#\n#..O..#\n#.....#\n#######\n\n<vv<<^^<<^^\n";
    std::string filedata = "##########\n#........#\n#O#......#\n#O.......#\n#@.......#\n#........#\n##########\n\n>^^^\n";
    std::stringstream file(filedata);
#endif
    std::string line, moves, line2;
    std::vector<std::string> map, map2;
    Vec2 robot(0, 0), robot2(0, 0);
    while(std::getline(file, line))
    {
        if (line[0] == '#')
        {
            // populate map for part 1
            map.push_back(line);

            // get start coordinates
            size_t pos = line.find('@');
            if (pos != std::string::npos)
            {
                robot = Vec2(pos, map.size()-1);
                map.back()[pos] = '.';
            }

            // populate map for part2
            line2.clear();
            int i = 0;
            for (const char& c : line)
            {
                if (c == '#') line2 += "##";
                else if (c == '.') line2 += "..";
                else if (c == 'O') line2 += "[]";
                else if (c == '@')
                {
                    robot2 = Vec2(2*i, map2.size());
                    line2 += "..";
                }
                ++i;
            }
            map2.push_back(line2);
        }
        else if (line.empty()) continue;
        else moves += line;
    }

    auto print = [](const Vec2& r, const std::vector<std::string>& map)
    {
        const int width = map[0].size(), height = map.size();
        for (int i = 0; i < height; ++i)
        {
            std::string tmp = map[i];
            if (i == r.y) tmp[r.x] = '@';
            std::cout << tmp << std::endl;
        }
    };

    auto eval = [](const std::vector<std::string>& map, const char& box)
    {
        size_t res = 0ul;
        const int w = map[0].size(), h = map.size();
        for (int i = 0; i < h; ++i)
            for (int j = 0; j < w; ++j)
                res += bool(map[i][j] == box) * (100 * i + j);
        return res;
    };

    const double t1 = t.micro().count(); t.reset();

    // part 1
    for (const char& move : moves)
    {
        const Vec2 next = robot.next(move);
        if (push(map, next, move))
            robot = next;
    }
    const size_t part1 = eval(map, 'O');
    const double t2 = t.micro().count(); t.reset();

    // part 2
    Part2 p2(map2);
    for (const char& move : moves)
    {
        const Vec2 next2 = robot2.next(move);
        if (p2.move(next2, move))
            robot2 = next2;
    }
    const size_t part2 = eval(map2, '[');
    const double t3 = t.micro().count(); t.reset();

    printf("Data loading time: %f µs\n", t1);
    printf("Part 1: %zu, Time: %f µs\n", part1, t2);
    printf("Part 2: %zu, Time: %f µs\n", part2, t3);
    return 0;
}