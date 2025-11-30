#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <algorithm>

namespace
{
using Memory = std::vector<uint32_t>;
size_t hash(Memory const& mem)
{
	size_t seed = mem.size();
	for (auto const &i : mem) {
		seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	return seed;
}

std::pair<uint32_t, uint32_t> defrag(Memory memory)
{
	std::unordered_set<size_t> hashes;
	std::vector<size_t> hash_order;
	uint32_t cycles = 0;
	size_t index = 0;
	uint32_t largest_block;

	do {
		auto const hash_val = hash(memory);
		hashes.insert(hash_val);
		hash_order.push_back(hash_val);
		largest_block = *std::max_element(memory.begin(), memory.end());
		auto it = std::ranges::find(memory, largest_block);
		uint32_t blocks = *it;
		*it = 0;

		while (blocks > 0) {
			it++;
			if (it == memory.end())
				it = memory.begin();
			*it += 1;
			blocks--;
		}
		cycles++;
	} while (!hashes.contains(hash(memory)));

	auto const hash_it = std::ranges::find(hash_order, hash(memory));
	uint32_t const loop_size = std::distance(hash_it, hash_order.end());
	return {cycles, loop_size};
}
}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	auto infile = std::ifstream("input.txt");
	assert(infile.good());

	std::vector<uint32_t> memory;
	uint32_t op;
	while (infile >> op) {
		memory.push_back(op);
	}

	auto const result = defrag(memory);
	std::cout << "Part 1: " << result.first << std::endl;
	std::cout << "Part 2: " << result.second << std::endl;

	return 0;
}
