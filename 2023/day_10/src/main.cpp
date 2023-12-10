
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <numeric>
#include <optional>
#include <set>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;

typedef std::pair<int, int> Dir;
typedef std::pair<int, int> Pos;

static std::vector<string> read_lines()
{
    std::ifstream in("input.txt");

    std::vector<string> lines;
    string line;
    while (std::getline(in, line)) {
        lines.push_back(line);
    }

    in.close();
    return lines;
}

static std::optional<std::pair<Dir, Dir>> get_dirs(const char& pipe)
{
    switch (pipe) {
        case '|':
            return std::optional<std::pair<Dir, Dir>>{std::pair(Dir(1,0), Dir(-1, 0))};
        case '-':
            return std::optional<std::pair<Dir, Dir>>{std::pair(Dir(0,1), Dir(0, -1))};
        case 'L':
            return std::optional<std::pair<Dir, Dir>>{std::pair(Dir(-1,0), Dir(0, 1))};
        case 'J':
            return std::optional<std::pair<Dir, Dir>>{std::pair(Dir(-1,0), Dir(0, -1))};
        case '7':
            return std::optional<std::pair<Dir, Dir>>{std::pair(Dir(1,0), Dir(0, -1))};
        case 'F':
            return std::optional<std::pair<Dir, Dir>>{std::pair(Dir(1,0), Dir(0, 1))};
        default:
            return std::nullopt;
    };
}

static std::optional<Dir> get_next_dir(const std::vector<string>& map, const Pos& pos, const Dir& in_dir)
{
    auto next_dirs = get_dirs(map[pos.first][pos.second]);

    if (!next_dirs)
        return std::nullopt;

    // Ensure incoming direction is valid
    auto dir1 = next_dirs->first;
    auto dir2 = next_dirs->second;

    std::optional<Pos> result = std::nullopt;
    if (dir1.first + in_dir.first == 0 && dir1.second + in_dir.second == 0) {
        result = std::optional<Dir>{dir2};
    } else if (dir2.first + in_dir.first == 0 && dir2.second + in_dir.second == 0) {
        result = std::optional<Pos>{dir1};
    }

    return result;
}

static std::optional<std::pair<Pos, Dir>> walk(const std::vector<string>& map, const Pos& pos, const Dir& dir)
{
    Pos next = Pos{pos.first + dir.first, pos.second + dir.second};

    if (next.first < 0 || next.first > (int) map.size())
        return std::nullopt;

    if (next.second < 0 || next.second >= (int) map[next.first].length())
        return std::nullopt;

    auto next_dirs = get_dirs(map[next.first][next.second]);

    if (!next_dirs)
        return std::nullopt;

    auto ndir = get_next_dir(map, next, dir);

    if (!ndir)
        return std::nullopt;

    return std::optional<std::pair<Pos, Dir>>{std::pair<Pos, Dir>(next, *ndir)};
}

static std::optional<std::vector<Pos>> map_walk(const std::vector<string>& map, const Pos& start, const Dir& start_dir)
{
    std::vector<Pos> positions{start};
    auto next = walk(map, start, start_dir);
    while (next) {
        auto pos = next->first;
        auto ndir = next->second;
        positions.push_back(pos);
        if (pos.first + ndir.first == start.first && pos.second + ndir.second == start.second) {
            return positions;
        }
        next = walk(map, pos, ndir);
    }

    return std::nullopt;
}

static void solve(const std::vector<string>& map, const Pos& start)
{
    std::vector<Dir> start_dirs{
        Dir{-1, 0},
        Dir{1, 0},
        Dir{0, -1},
        Dir{0, 1},
    };

    std::vector<Pos> path;
    for (const auto& d : start_dirs) {
        auto steps = map_walk(map, start, d);
        if (steps) {
            cout << "Part 1: " << steps->size()/2 << endl;
            path = *steps;
            break;
        }
    }

    std::set<Pos> positions(path.begin(), path.end());

    size_t contained = 0;
    for (size_t i = 0; i < map.size(); ++i) {
            size_t walls = 0;
        for (size_t j = 0; j < map[i].length(); ++j) {
            char c = map[i][j];
            if (positions.find(Pos(i, j)) == positions.end()) {
                if ((walls % 2) == 1)
                    contained++;
            } else {
                if (c == '|' || c == 'F' || c == '7' || c == 'S') 
                    walls++;
            }
        }
    }

    cout << "Part 2: " << contained << endl;

    /*
    // Print the "map"
    size_t j = 0;
    for (auto& r : map) {
        for (size_t i = 0; i < r.length(); ++i) {
            Pos pos{j, i};
            if (positions.find(pos) != positions.end()) {
                cout << "\x1b[31m" << r[i] << "\x1b[0m";
            } else {
                cout << r[i];
            }
        }
        j++;
        cout << endl;
    }
    */
}

int main()
{

    auto lines = read_lines();

    // Find 'S'
    int r = 0, c = 0;
    for (size_t i = 0; i < lines.size(); ++i) {
        auto pos = lines[i].find('S');
        if (pos != std::string::npos) {
            r = i;
            c = pos;
            break;
        }
    }

    Pos start(r, c);
    solve(lines, start);

    return 0;
}
