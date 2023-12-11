
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

static std::set<int> find_empty_rows(const std::vector<string>& space)
{
    std::set<int> empty_rows;
    for (size_t i = 0; i < space.size(); ++i) {
        auto& row = space[i];
        if (std::all_of(row.begin(), row.end(), [](const auto& c) { return c == '.'; })) {
            empty_rows.insert(i);
        }
    }
    return empty_rows;
}

static std::set<int> find_empty_cols(const std::vector<string>& space)
{
    std::set<int> empty_cols;
    for (size_t i = 0; i < space[0].size(); ++i) {
        if (std::all_of(space.begin(), space.end(), [&i](const auto& row) { return row[i] == '.'; })) {
            empty_cols.insert(i);
        }
    }
    return empty_cols;
}

struct Pos {
    int r;
    int c;

    bool operator<(const Pos& rhs) const {
        if (r == rhs.r) {
            return c < rhs.c;
        } else {
            return r < rhs.r;
        }
    }

    bool operator==(const Pos& other) const {
        return c == other.c && r == other.r;
    }

    friend std::ostream& operator<<(std::ostream& os, const Pos& p);
};

std::ostream& operator<<(std::ostream& os, const Pos& p)
{
    os << p.r << "," << p.c;
    return os;
}

static std::set<Pos> find_galaxies(const std::vector<string>& space)
{
    std::set<Pos> galaxies;
    for (size_t i = 0; i < space.size(); ++i) {
        for (size_t j = 0; j < space[i].length(); ++j) {
            if (space[i][j] == '#') {
                galaxies.emplace(static_cast<int>(i), static_cast<int>(j));
            }
        }
    }
    return galaxies;
}

static int get_distance(
        const Pos& p1,
        const Pos& p2,
        const std::set<int>& empty_cols,
        const std::set<int>& empty_rows,
        int age_mod)
{
    int r1 = std::min(p1.r, p2.r);
    int r2 = std::max(p1.r, p2.r);

    int r_dist = 0;
    for (int i = r1; i < r2; ++i) {
        if (empty_rows.contains(i)) {
            r_dist += age_mod;
        } else {
            r_dist++;
        }
    }

    int c1 = std::min(p1.c, p2.c);
    int c2 = std::max(p1.c, p2.c);

    int c_dist = 0;
    for (int i = c1; i < c2; ++i) {
        if (empty_cols.contains(i)) {
            c_dist += age_mod;
        } else {
            c_dist++;
        }
    }
    return c_dist + r_dist;
}

static void solve(
        const std::vector<string>& space,
        const std::set<int>& empty_cols,
        const std::set<int>& empty_rows,
        const std::set<Pos>& galaxies)
{
    std::set<std::pair<Pos, Pos>> distances;

    size_t total1 = 0;
    size_t total2 = 0;
    for (auto it1 = galaxies.begin(); it1 != galaxies.end(); it1++) {
        const auto& g1 = *it1;
        for (auto it2 = std::next(it1); it2 != galaxies.end(); it2++) {
            const auto& g2 = *it2;

            total1 += get_distance(g1, g2, empty_cols, empty_rows, 2);
            total2 += get_distance(g1, g2, empty_cols, empty_rows, 1000000);
        }
    }

    cout << "Part 1: " << total1 << endl;
    cout << "Part 2: " << total2 << endl;
}

int main()
{

    auto space = read_lines();

    auto empty_rows = find_empty_rows(space);
    auto empty_cols = find_empty_cols(space);
    auto galaxies = find_galaxies(space);

    solve(space, empty_cols, empty_rows, galaxies);

    return 0;
}
