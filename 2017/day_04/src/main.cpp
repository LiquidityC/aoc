#include <iostream>
#include <set>
#include <fstream>
#include <sstream>
#include <cstdint>

bool has_duplicate_word(std::string const& line)
{
	std::set<std::string> words;
	std::istringstream ss(line);
	std::string word;
	while (ss >> word) {
		if (words.contains(word)) {
			return true;
		}
		words.insert(word);
	}
	return false;
}

bool has_duplicate_anagram(std::string const& line)
{
	std::set<std::string> words;
	std::istringstream ss(line);
	std::string word;
	while (ss >> word) {
		std::sort(word.begin(), word.end());
		if (words.contains(word)) {
			return true;
		}
		words.insert(word);
	}
	return false;
}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	auto infile = std::ifstream("input.txt");

	uint32_t result1 = 0;
	uint32_t result2 = 0;
	std::string line;
	while (std::getline(infile, line)) {
		result1 += has_duplicate_word(line) ? 0u : 1u;
		result2 += has_duplicate_anagram(line) ? 0u : 1u;
	}

	std::cout << "Part 1: " << result1 << std::endl;
	std::cout << "Part 2: " << result2 << std::endl;
	return 0;
}
