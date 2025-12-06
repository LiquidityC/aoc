#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <ranges>
#include <cassert>

namespace rv = std::ranges::views;
using u64 = uint64_t;

using Problem = std::vector<u64>;
using Problems = std::vector<Problem>;
using Operators = std::vector<char>;

namespace
{
u64 solve(Problem const& problem, char op)
{
	u64 total = problem.front();
	for (auto num : problem | rv::drop(1)) {
		if (op == '*') {
			total *= num;
		} else {
			total += num;
		}
	}
	return total;
}

void part1()
{
	Problems problems;
	Operators operators;

	auto infile = std::ifstream("input.txt");
	std::string line;
	while (std::getline(infile, line)) {
		if (line.empty())
			break;

		u64 index = 0;
		if (line.find('+') == std::string::npos && line.find('*') == std::string::npos) {
			u64 num;
			std::istringstream iss(line);
			while (iss >> num) {
				if (index >= problems.size()) {
					problems.emplace_back();
				}
				problems.at(index).push_back(num);
				index++;
			}
		} else {
			for (auto c : line) {
				if (c != ' ') {
					operators.push_back(c);
				}
			}
		}
	}

	u64 total = 0;
	for (auto [i, p] : problems | rv::enumerate) {
		char op = operators.at(i);
		total += solve(p, op);
	}
	std::cout << "Part 1: " << total << std::endl;
}

void part2()
{
	std::vector<std::string> lines;
	Problems problems;
	Operators operators;

	auto infile = std::ifstream("input.txt");
	std::string line;
	size_t line_len = 0;

	// Build a character matrix and ensure all the lines have the same length
	// Exclude the operator line from the matrix.
	while (std::getline(infile, line)) {
		if (line.empty())
			break;

		assert(line_len == 0 || line_len == line.length());
		line_len = line.length();

		if (line.find('+') == std::string::npos && line.find('*') == std::string::npos) {
			lines.push_back(std::move(line));
		} else {
			for (auto c : line) {
				if (c != ' ') {
					operators.push_back(c);
				}
			}
		}
	}

	// Reverse the operators since we're reading right to left
	std::reverse(operators.begin(), operators.end());

	// Parse the character matrix left to right, top to bottom
	problems.emplace_back();
	for (int i = line_len - 1; i >= 0; --i) {
		std::string sum = "";
		for (auto line : lines) {
			char c = line[i];
			if (c != ' ') {
				sum.push_back(c);
			}
		}
		if (sum.empty()) {
			problems.emplace_back();
			continue;
		} else {
			problems.back().push_back(std::stoll(sum));
		}
	}

	u64 total = 0;
	for (auto [i, p] : problems | rv::enumerate) {
		char op = operators.at(i);
		total += solve(p, op);
	}
	std::cout << "Part 2: " << total << std::endl;
}

}  // namespace 

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	part1();
	part2();
}
