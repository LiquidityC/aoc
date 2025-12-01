#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <ranges>

namespace
{
namespace rv = std::ranges::views;
using Rotations = std::vector<int>;

uint32_t calculate_pass(Rotations const& rotations)
{
	int dial = 50;
	uint32_t zero_count = 0;
	for (int turn : rotations) {
		int const next = dial + turn;
		dial = next % 100 + (next % 100 < 0 ? 100 : 0);
		if (dial == 0) {
			zero_count++;
		}
	}

	return zero_count;
}

uint32_t alt_calculate_pass(Rotations const& rotations)
{
	uint32_t zero_count = 0;
	int dial = 50;

	for (auto turn : rotations) {
		int const delta = std::abs(turn);
		int const inc = turn < 0 ? -1 : 1;
		for (auto _ : rv::iota(0, delta)) {
			dial += inc;
			dial = dial % 100 + ((dial % 100) < 0 ? 100 : 0);
			if (dial == 0) {
				zero_count++;
			}
		}
	}

	return zero_count;
}

}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	Rotations rotations;

	auto infile = std::ifstream("input.txt");

	std::string line;
	while (std::getline(infile, line)) {
		if (line[0] == 'L') {
			line[0] = '-';
		} else {
			line[0] = ' ';
		}

		std::istringstream iss(line);
		int num;
		iss >> num;
		rotations.push_back(num);
	}


	std::cout << "Part 1: " << calculate_pass(rotations) << std::endl;
	std::cout << "Part 2: " << alt_calculate_pass(rotations) << std::endl;
}
