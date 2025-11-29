#include <iostream>
#include <fstream>
#include <vector>

namespace
{
int run_program(std::vector<int> program)
{
	int steps = 0;
	int index = 0;
	const int prog_size = static_cast<int>(program.size());
	while (index >= 0 && index < prog_size) {
		index += program[index]++;
		steps++;
	}
	return steps;
}

int alt_run_program(std::vector<int> program)
{
	int steps = 0;
	int index = 0;
	const int prog_size = static_cast<int>(program.size());
	while (index >= 0 && index < prog_size) {
		int &val = program[index];
		index += val;
		steps++;
		if (val >= 3)
			val--;
		else
			val++;
	}
	return steps;
}
}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	auto infile = std::ifstream("input.txt");

	std::vector<int> program;
	int op;
	while (infile >> op) {
		program.push_back(op);
	}

	std::cout << "Part 1: " << run_program(program) << std::endl;
	std::cout << "Part 2: " << alt_run_program(program) << std::endl;
}
