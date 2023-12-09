
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <map>
#include <numeric>
#include <string>
#include <vector>
#include <sstream>

using std::cout;
using std::endl;
using std::string;

static std::vector<std::vector<int>> read_lines()
{
    std::ifstream in("input.txt");

    std::vector<std::vector<int>> data;
    string line;
    while (std::getline(in, line)) {
        std::vector<int> numbers;
        std::stringstream ss(line);
        int num;
        while (ss >> num) {
            numbers.push_back(num);
        }
        data.push_back(numbers);
    }

    in.close();
    return data;
}

static int extrapolate_last(std::vector<int>& numbers)
{
    if (std::all_of(numbers.begin(), numbers.end(), [](const auto& a) { return a == 0; })) {
        return 0;
    }

    std::vector<int> next;
    next.reserve(numbers.size() - 1);
    for (size_t i = 0; i < numbers.size() - 1; ++i) {
        next.push_back(numbers[i+1] - numbers[i]);
    }
    return numbers.back() + extrapolate_last(next);
}

static void part1(std::vector<std::vector<int>>& data)
{
    int total = 0;
    for (auto& numbers : data) {
        int next = extrapolate_last(numbers);
        total += next;
    }
    cout << "Part 1: " << total << endl;
}

static int extrapolate_first(std::vector<int>& numbers)
{
    if (std::all_of(numbers.begin(), numbers.end(), [](const auto& a) { return a == 0; })) {
        return 0;
    }

    std::vector<int> next;
    next.reserve(numbers.size() - 1);
    for (size_t i = 0; i < numbers.size() - 1; ++i) {
        next.push_back(numbers[i+1] - numbers[i]);
    }
    return numbers.front() - extrapolate_first(next);
}

static void part2(std::vector<std::vector<int>>& data)
{
    int total = 0;
    for (auto& numbers : data) {
        int next = extrapolate_first(numbers);
        total += next;
    }
    cout << "Part 2: " << total << endl;
}

int main()
{

    auto data = read_lines();

    part1(data);
    part2(data);

    return 0;
}
