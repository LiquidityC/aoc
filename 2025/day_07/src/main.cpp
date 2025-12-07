#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>
#include <ranges>
#include <cstdint>

namespace rv = std::ranges::views;

using u64 = uint64_t;

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	std::vector<u64> streams;
	u64 timelines = 1;
	u64 splits = 0;
	std::string line;

	auto infile = std::ifstream("input.txt");
	while (std::getline(infile, line) && !line.empty()) {
		// Populate the streams vector from the first row.
		if (streams.empty()) {
			for (auto c : line) {
				if (c == 'S') {
					streams.push_back(1);
				} else {
					streams.push_back(0);
				}
			}
		} else {
			// Parse the line and accumulate streams, splits and timelines
			for (auto [i, c] : line | rv::enumerate) {
				auto const stream_count = streams[i];
				if (c == '^' && stream_count > 0) {
					splits++;
					streams[i-1] += stream_count; // Set the left split
					streams[i] = 0; // Zero out the stream on/under the splitter
					streams[i+1] += stream_count; // Set the right split
					timelines += stream_count; // Every "right split" create a new timeline
				}
			}
		}
	}

	std::cout << "Part 1: " << splits << std::endl;
	std::cout << "Part 2: " << timelines << std::endl;

	return 0;
}
