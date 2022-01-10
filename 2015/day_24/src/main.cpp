#include <iostream>
#include <vector>
#include <fstream>
#include <climits>

/*
 * Brute force solution. Divide the total weight of packages by the available
 * compartments (3, for part 1, 4 for part 2). Iterate a bitmask to create all possible combinations
 * for one compartment. Keep track of the wieght and sum of that compartment. Keep the best result.
 */
uint64_t balance(std::vector<uint32_t> packages, uint32_t parts)
{
	uint32_t total_sum = 0;
	for (auto n : packages)
		total_sum += n;

	uint32_t part = total_sum / parts;
	uint32_t best_ncount = 100;
	uint64_t best_entangle = UINT64_MAX;
	size_t ccount = packages.size();

	for (auto i = 1; i < (1 << ccount); ++i) {
		uint32_t ncount = 0;
		uint64_t entangle = 1;
		uint32_t sum = 0;

		if (__builtin_popcount(i) > static_cast<int>(best_ncount))
			continue;

		for (size_t j = 0; j < ccount; ++j) {
			if (((i >> j) & 1) == 1) {
				ncount++;
				sum += packages[j];
				entangle *= packages[j];
			}
			if (sum > part || entangle > best_entangle)
				break;
		}
		if (sum == part) {
			if (ncount < best_ncount) {
				best_ncount = ncount;
				best_entangle = entangle;
			} else if (best_ncount == ncount && best_entangle > entangle) {
				best_entangle = entangle;
			}
		}
	}
	return best_entangle;
}

int main()
{
	std::vector<uint32_t> numbers;
	std::ifstream input("input.txt");
	uint32_t n;
	while (input >> n) {
		numbers.push_back(n);
	}
	std::cout << "Part 1: " << balance(numbers, 3) << std::endl;
	std::cout << "Part 2: " << balance(numbers, 4) << std::endl;

	return 0;
}
