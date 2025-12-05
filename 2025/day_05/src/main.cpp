#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>
#include <sstream>
#include <cstdint>
#include <set>
#include <utility>
#include <ranges>

using u64 = uint64_t;
using Range = std::pair<u64, u64>;
using Ranges = std::set<Range>;
using Serials = std::vector<u64>;

namespace rv = std::ranges::views;

namespace
{
size_t count_valid_serials(Serials const& serials, Ranges const& ranges)
{
	size_t total = 0;
	for (auto const serial : serials) {
		for (auto const& range : ranges) {
			if (serial >= range.first && serial <= range.second) {
				//std::cout << std::format("{} is fresh [{}-{}]", serial, range.first, range.second) << std::endl;
				total++;
				break;
			}
		}
	}
	return total;
}
}  // namespace


int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	auto infile = std::ifstream("input.txt");
	std::string line;

	Ranges ranges;
	Serials serials;

	bool parsing_ranges = true;
	while (std::getline(infile, line)) {
		if (line.empty()) {
			if (parsing_ranges) {
				parsing_ranges = false;
				continue;
			} else {
				break;
			}
		}

		std::istringstream iss(line);
		if (parsing_ranges) {
			u64 begin;
			u64 end;
			iss >> begin;
			iss.get(); // pop '-'
			iss >> end;

				ranges.emplace(begin, end);
		} else {
			u64 serial;
			iss >> serial;
			serials.push_back(serial);
		}
	}

	// Merge all the ranges to remove overlaps (Ranges are sorted already because of std::set
	auto current = *ranges.begin();
	Ranges merged_ranges;
	for (auto range : ranges | rv::drop(1)) {
		if (range.first <= current.second && range.second > current.second) { // The next range extends the current one
			current.second = range.second;
		} else if (range.first > current.second) { // The next range is beyond the current one
			merged_ranges.insert(current);
			current = range;
		}
	}
	merged_ranges.insert(current); // Insert the last range

	std::cout << "Part 1: " << count_valid_serials(serials, merged_ranges) << std::endl;
	std::cout << "Part 2: " << std::accumulate(merged_ranges.begin(), merged_ranges.end(), 0ull, [](u64 total, auto range) {
			return total + (range.second + 1) - range.first;
			}) << std::endl;
	return 0;
}
