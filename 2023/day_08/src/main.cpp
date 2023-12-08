#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <numeric>
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

typedef std::pair<string, string> Dir;

static void part1(std::map<string, Dir>& network, std::string& instructions)
{
    std::string node = "AAA";
    size_t steps = 0;
    auto it = instructions.begin();
    while (node != "ZZZ") {
        steps++;
        const auto& opt = network[node];
        if (*it == 'L') {
            node = opt.first;
        } else {
            node = opt.second;
        }
        it++;
        if (it == instructions.end()) {
            it = instructions.begin();
        }
    }

    cout << "Part 1: " << steps << endl;
}

static size_t count_steps_for_node(const string& start, std::map<string, Dir>& network, std::string& instructions)
{
    std::string node = start;
    size_t steps = 0;
    auto it = instructions.begin();
    while (node[2] != 'Z') {
        steps++;
        const auto& opt = network[node];
        if (*it == 'L') {
            node = opt.first;
        } else {
            node = opt.second;
        }
        it++;
        if (it == instructions.end()) {
            it = instructions.begin();
        }
    }

    return steps;
}

static void part2(std::map<string, Dir>& network, std::string& instructions)
{
    std::vector<string> nodes;

    for (auto& it : network) {
        if (it.first[2] == 'A') {
            nodes.push_back(it.first);
        }
    }

    size_t steps = 1;
    for (const auto& n : nodes) {
        steps = std::lcm(steps, count_steps_for_node(n, network, instructions));
    }
    cout << "Part 2: " << steps << endl;
}

int main()
{

    auto lines = read_lines();

    string instructions = lines[0];
    std::map<string, Dir> network;
    for (size_t i = 2; i < lines.size(); ++i) {
        auto& line = lines[i];
        auto delim = line.find(" = ");
        auto key = line.substr(0, delim);
        auto pairs = line.substr(delim + 3);
        auto left = pairs.substr(1, 3);
        auto right = pairs.substr(6, 3);

        network[key] = Dir(left, right);
    }


    part1(network, instructions);
    part2(network, instructions);

    return 0;
}
