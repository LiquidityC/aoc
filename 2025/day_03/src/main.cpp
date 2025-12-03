#include <iostream>
#include <fstream>
#include <vector>
#include <ranges>

namespace rv = std::ranges::views;

using Bank = std::vector<uint64_t>;
using Banks = std::vector<Bank>;

namespace 
{

uint64_t joltage(std::span<uint64_t> bank)
{
	uint64_t res = 0;
	for (auto bat : bank) {
		res = res * 10 + bat;
	}
	return res;
}

uint64_t get_max_bank_bat_joltage(Bank const& bank, size_t bat_count)
{
	uint64_t max_depth = bank.size() - bat_count;

	Bank stack;
	for (auto bat : bank) {
		while (!stack.empty() && stack.back() < bat && max_depth > 0) {
			stack.pop_back();
			max_depth -= 1;
		}
		stack.push_back(bat);
	}

	return joltage(stack | rv::take(bat_count));
}

uint64_t find_max_bat_joltage(Banks const& banks, size_t bat_count)
{
	uint64_t sum = 0;
	for (auto const &bank : banks) {
		uint64_t const best = get_max_bank_bat_joltage(bank, bat_count);
		sum += best;
	}
	return sum;
}

}  // namespace 


int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	Banks banks;

	auto infile = std::ifstream("input.txt");
	std::string line;
	while (std::getline(infile, line)) {
		banks.emplace_back();
		for (auto c : line) {
			banks.back().push_back(c - '0');
		}
	}

	std::cout << "Part 1: " << find_max_bat_joltage(banks, 2) << std::endl;
	std::cout << "Part 2: " << find_max_bat_joltage(banks, 12) << std::endl;

	return 0;
}
