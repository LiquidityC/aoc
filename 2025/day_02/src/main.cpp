#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <ranges>
#include <unordered_set>

using Serials = std::vector<std::pair<uint64_t, uint64_t>>;
namespace rv = std::ranges::views;

namespace
{
size_t num_digits(uint64_t number)
{
	size_t count = 0;
	while (number > 0) {
		number /= 10;
		count++;
	}
	return count;
}

bool is_palindrome(uint64_t const num)
{
	size_t power = num_digits(num) / 2;
	uint64_t op = std::pow(10, power);

	uint64_t second_half = num % op;
	uint64_t first_half = (num - second_half) / op;

	return first_half == second_half;
}

uint64_t count_invalid(Serials const& serials)
{

	uint64_t total = 0;
	for (auto const& [from, to] : serials) {
		for (auto const num : rv::iota(from, to+1) | rv::filter(is_palindrome)) {
			total += num;
		}
	}
	return total;
}

bool has_repeating_pattern(uint64_t num, uint64_t pattern_len)
{
	uint64_t op = std::pow(10, pattern_len);

	std::unordered_set<uint64_t> patterns;
	while (num > 0) {
		uint64_t pattern = num % op;
		if (num_digits(pattern) != pattern_len)
			return false;
		patterns.insert(pattern);
		if (patterns.size() > 1) {
			return false;
		}
		num = (num - pattern) / op;
	}

	return patterns.size() == 1;
}

bool has_pattern(uint64_t const num)
{
	if (num < 10)
		return false;

	uint64_t max_len = num_digits(num)/2;
	for (uint64_t i = 1; i <= max_len; ++i) {
		if (has_repeating_pattern(num, i)) {
			return true;
		}
	}
	return false;
}

uint64_t alt_count_invalid(Serials const& serials)
{

	uint64_t total = 0;
	for (auto const& [from, to] : serials) {
		for (auto const num : rv::iota(from, to+1) | rv::filter(has_pattern)) {
			total += num;
		}
	}
	return total;
}

}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	auto infile = std::ifstream("input.txt");

	Serials serials;
	while (!infile.eof() && infile.peek() != -1) {
		uint64_t from, to;
		infile >> from;
		(void) infile.get(); // Pop the '-'
		infile >> to;
		serials.emplace_back(from, to);

		if (infile.eof() || infile.peek() == -1)
			break;
		(void) infile.get(); // Pop the ','
	}

	std::cout << "Part 1: " << count_invalid(serials) << std::endl;
	std::cout << "Part 2: " << alt_count_invalid(serials) << std::endl;

	return 0;
}
