#include <iostream>
#include <limits>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <ranges>
#include <cmath>
#include <algorithm>

namespace rv = std::ranges::views;
using u64 = uint64_t;
using i64 = int64_t;

namespace
{

struct Vertex
{
	i64 group_size = 1; // Placed first for sorting

	i64 x;
	i64 y;
	i64 z;

	Vertex *parent = nullptr;

	Vertex *get_root()
	{
		Vertex *root = this;
		while (root->parent) {
			root = root->parent;
			parent = root;
		}

		return root;
	}

	u64 distance(Vertex const& other) const
	{
		return std::sqrt(std::pow(std::abs(x - other.x), 2)
			+ std::pow(std::abs(y-other.y), 2)
			+ std::pow(std::abs(z-other.z), 2));
	}

	auto operator<=>(const Vertex&) const = default;

	friend std::istream& operator>>(std::istream& is, Vertex& box);
};

struct Edge
{
	u64 distance = std::numeric_limits<u64>::max(); // Placed first for sorting

	Vertex *a = nullptr;
	Vertex *b = nullptr;

	auto operator<=>(const Edge&) const = default;
};

std::istream& operator>>(std::istream& is, Vertex& box) {
	std::string line;

	if (!getline(is, line)) {
		is.setstate(std::ios::eofbit);
		return is;
	}
	std::istringstream iss(line);
	iss >> box.x;
	assert(',' == iss.get());
	iss >> box.y;
	assert(',' == iss.get());
	iss >> box.z;

	if (is.peek() == -1) {
		is.setstate(std::ios::eofbit);
		return is;
	}
	return is;
}

void join(const Edge& e)
{
	Vertex *a_root = e.a->get_root();
	Vertex *b_root = e.b->get_root();

	if (a_root != b_root) {
		a_root->group_size += b_root->group_size;
		b_root->group_size = 0;
		b_root->parent = a_root;
	}
}

std::vector<std::pair<size_t, size_t>> all_pairs(size_t max)
{
	std::vector<std::pair<size_t, size_t>> pairs;
	pairs.reserve(max*max);
	for (size_t i = 0; i < max; ++i) {
		for (size_t j = i+1; j < max; ++j) {
			pairs.emplace_back(i, j);
		}
	}
	return pairs;
}

std::vector<Edge> generate_all_edges(std::vector<Vertex>& vertices)
{
	// Calculate all edges
	std::vector<Edge> edges;
	edges.reserve(vertices.size() * vertices.size() + 1);
	for (auto const& p : all_pairs(vertices.size())) {
		auto const dist = vertices[p.first].distance(vertices[p.second]);
		edges.push_back({ dist, &vertices[p.first], &vertices[p.second] });
	}
	std::ranges::sort(edges);
	return edges;
}

void part1(std::vector<Vertex> vertices, size_t count)
{

	auto edges = generate_all_edges(vertices);

	for (u64 i = 0; i < count; ++i) {
		join(edges[i]);
	}
	std::ranges::sort(vertices); // Invalidates edges
	u64 result = std::ranges::fold_left(vertices
			| rv::reverse
			| rv::take(3)
			| rv::transform([](auto const& v) { return v.group_size; }),
			1,
			std::multiplies{});

	std::cout << "Part 1: " << result << std::endl;
}

void part2(std::vector<Vertex> vertices)
{
	auto edges = generate_all_edges(vertices);

	// Keep joining until we have one giant circuit
	for (u64 i = 0; i < edges.size(); ++i) {
		auto const& edge = edges[i];
		join(edge);
		u64 const gsize = edge.a->group_size;
		if (gsize == vertices.size()) {
			std::cout << "Part 2: " << edge.a->x * edge.b->x << std::endl;
			break;
		}
	}
}

}  // namespace


int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	std::vector<Vertex> vertices;

	auto infile = std::ifstream("input.txt");
	do {
		infile >> vertices.emplace_back();
	} while (!infile.eof());

	part1(vertices, 1000);
	part2(vertices);
}
