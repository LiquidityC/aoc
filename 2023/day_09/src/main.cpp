
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

typedef uint32_t u32;

static std::vector<std::vector<u32>> read_lines()
{
    std::ifstream in("input.txt");

    std::vector<std::vector<u32>> data;
    string line;
    while (std::getline(in, line)) {
        std::vector<u32> numbers;
        std::stringstream ss(line);
        u32 num;
        while (ss >> num) {
            numbers.push_back(num);
        }
        data.push_back(numbers);
    }

    in.close();
    return data;
}

static u32 extrapolate_last(std::vector<u32>& numbers)
{
    if (std::all_of(numbers.begin(), numbers.end(), [](const auto& a) { return a == 0; })) {
        return 0;
    }

    std::vector<u32> next;
    next.reserve(numbers.size() - 1);
    for (size_t i = 0; i < numbers.size() - 1; ++i) {
        next.push_back(numbers[i+1] - numbers[i]);
    }
    return numbers.back() + extrapolate_last(next);
}

static void part1(std::vector<std::vector<u32>>& data)
{
    u32 total = 0;
    for (auto& numbers : data) {
        u32 next = extrapolate_last(numbers);
        total += next;
    }
    cout << "Part 1: " << total << endl;
}

static int extrapolate_first(std::vector<u32>& numbers)
{
    if (std::all_of(numbers.begin(), numbers.end(), [](const auto& a) { return a == 0; })) {
        return 0;
    }

    std::vector<u32> next;
    next.reserve(numbers.size() - 1);
    for (size_t i = 0; i < numbers.size() - 1; ++i) {
        next.push_back(numbers[i+1] - numbers[i]);
    }
    return numbers.front() - extrapolate_first(next);
}

static void part2(std::vector<std::vector<u32>>& data)
{
    u32 total = 0;
    for (auto& numbers : data) {
        u32 next = extrapolate_first(numbers);
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
