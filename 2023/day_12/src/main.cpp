#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
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

typedef int64_t i64;
using VS = std::vector<i64>;
using VVS = std::vector<VS>;
using VVVS = std::vector<VVS>;

static size_t solve(std::string springs, const std::vector<int>& nums)
{
    std::function<size_t (int, int, int)> func;

    int max_len = *std::max_element(nums.begin(), nums.end());
    VVVS dp(springs.size(), VVS(nums.size()+1, VS(max_len+1, -1)));
    func = [&](size_t i, size_t j, int cur) -> size_t {
        // i = springs char index
        // j = nums index
        // cur = length of current run

        // If we are that end of springs. Check so we have the right number of '#' groups
        if (i >= springs.size()) {
            return j == nums.size();
        }

        // We already have this value
        if (dp[i][j][cur] != -1) {
            return dp[i][j][cur];
        }

        char cur_char = springs[i];
        auto& res = dp[i][j][cur];
        res = 0;
        
        // We aren't counting a group of '#'. Just move along
        if ((cur_char == '.' || cur_char == '?') && cur == 0)
            res += func(i+1, j, 0);

        // We are counting or should start counting a '#' group size
        if ((cur_char == '#' || cur_char == '?') && j < nums.size()) {

            // The current group is the right size
            if (cur+1 == nums[j]) {
                // Check ahead if it won't obviously be wrong to do so
                if (i+1 == springs.size() || springs[i+1] != '#') {
                    res += func(i+2, j+1, 0);
                }
            } else {
                // If not, keep going
                res += func(i+1, j, cur+1);
            }
        }
        return res;
    };

    return func(0, 0, 0);
}

static void part1(const std::vector<string>& lines)
{
    size_t total = 0;
    for (auto& l : lines) {
        size_t i = l.find(' ');
        auto springs = l.substr(0, i);
        auto conf = l.substr(i+1);

        std::vector<int> nums;
        std::stringstream ss(conf);
        int n;
        char c;
        while (ss >> n) {
            nums.push_back(n);
            ss >> c;
        }

        auto arrs = solve(springs, nums);
        total += arrs;
    }

    cout << "Part 1: " << total << endl;
}

static void part2(const std::vector<string>& lines)
{
    size_t total = 0;
    for (auto& l : lines) {
        size_t i = l.find(' ');
        auto springs = l.substr(0, i);
        auto conf = l.substr(i+1);

        auto s = springs;
        for (size_t i = 0; i < 4; ++i) {
            springs += "?" + s;
        }

        std::vector<int> nums;
        std::stringstream ss(conf);
        int n;
        char c;
        while (ss >> n) {
            nums.push_back(n);
            ss >> c;
        }

        auto v = nums;
        for (size_t i = 0; i < 4; ++i) {
            for (auto x : v) {
                nums.push_back(x);
            }
        }

        auto arrs = solve(springs, nums);
        total += arrs;
    }

    cout << "Part 2: " << total << endl;
}

int main()
{

    auto lines = read_lines();

    part1(lines);
    part2(lines);

    return 0;
}
