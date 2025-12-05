#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>
#include <cassert>
#include <ranges>

namespace rv = std::ranges::views;

namespace 
{
using Grid = std::vector<std::vector<bool>>;

size_t neighbor_count(Grid const& grid, int r, int c)
{
	assert(!grid.empty());
	static int const height = grid.size();
	static int const width = grid[0].size();

	size_t neighbor_count = 0;

	for (int i = std::max(0, r-1); i < std::min(height,r+2); ++i) {
		for (int j = std::max(0, c-1); j < std::min(width,c+2); ++j) {
			if (i == r && j == c)
				continue;
			neighbor_count += static_cast<size_t>(grid[i][j]);
		}
	}
	return neighbor_count;
}

size_t accessible_rolls(Grid const& grid)
{
	size_t total = 0;
	for (auto const& [r, row] : grid | rv::enumerate) {
		for (auto const& [c, roll] : row | rv::enumerate) {
			total += (roll && neighbor_count(grid, r, c) < 4 ? 1 : 0);
		}
	}

	return total;
}

size_t remove_rolls(Grid grid)
{
	auto remove_cycle = [](Grid& grid) {
		size_t total = 0;
		std::vector<std::pair<size_t, size_t>> marked_for_remove;
		for (auto const& [r, row] : grid | rv::enumerate) {
			for (auto const& [c, roll] : row | rv::enumerate) {
				bool const removeable = roll && neighbor_count(grid, r, c) < 4;
				total += removeable ? 1 : 0;
				if (removeable) {
					marked_for_remove.emplace_back(r, c);
				}
			}
		}
		for (auto [r, c] : marked_for_remove) {
			grid[r][c] = false;
		}
		return total;
	};

	size_t total = 0;
	size_t cycle;
	while ((cycle = remove_cycle(grid)) > 0) {
		total += cycle;
	}

	return total;
}

}  // namespace 


int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	Grid grid;

	auto infile = std::ifstream("input.txt");
	std::string line;
	while (std::getline(infile, line)) {
		if (line.empty())
			break;
		grid.emplace_back();
		for (auto c : line) {
			grid.back().push_back(c == '@');
		}
	}

	/*
	for (auto const& [i, row] : grid | rv::enumerate) {
		std::cout << std::format("[{}] ", i);
		for (auto b : row) {
			std::cout << (b ? '@' : '.');
		}
		std::cout << std::endl;
	}
	*/

	std::cout << "Part 1: " << accessible_rolls(grid) << std::endl;
	std::cout << "Part 2: " << remove_rolls(grid) << std::endl;

	return 0;
}
