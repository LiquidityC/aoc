#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <fstream>
#include <cstdint>
#include <ranges>
#include <vector>

namespace rv = std::ranges::views;
using i64 = int64_t;
using u64 = uint64_t;

namespace
{
struct Point
{
	i64 x;
	i64 y;

	auto operator<=>(Point const&) const = default;
};
struct Rectangle
{
	u64 area;
	Point a;
	Point b;

	Rectangle(Point const& a, Point const& b) : a(a), b(b) {
		area = (std::abs(a.x - b.x) + 1) * (std::abs(a.y - b.y) + 1);
	}

	auto operator<=>(Rectangle const&) const = default;
};

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

i64 orientation(Point const& a, Point const& b, Point const& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool point_on_segment(Point const& p, Point const& q, Point const& r)
{
	return q.x >= std::min(p.x, r.x)
		&& q.x <= std::max(p.x, r.x)
		&& q.y >= std::min(p.y, r.y)
		&& q.y <= std::max(p.y, r.y);
}

bool point_in_poly(std::vector<Point> const& points, Point const& p)
{
    bool inside = false;

    for(size_t i=0; i < points.size(); i++)
    {
        Point a = points[i];
        Point b = points[(i + 1) % points.size()];

        if(orientation(a, b, p) == 0 && point_on_segment(a, p, b)) {
			// Orientation is horizontal and point is on the line
            return true;
        }

        if((a.y > p.y) == (b.y > p.y)) {
            continue;
        }

        i64 dx = b.x - a.x;
        i64 dy = b.y - a.y;

        i64 lhs = (p.x - a.x) * dy;
        i64 rhs = (p.y - a.y) * dx;

        if((dy > 0 && lhs < rhs) || (dy < 0 && lhs > rhs)){
            inside = !inside;
        }
    }

    return inside;
}

bool has_intersection(Point const& p1, Point const& p2, Point const& q1, Point const& q2)
{
    i64 o1 = orientation(p1, p2, q1);
    i64 o2 = orientation(p1, p2, q2);
    i64 o3 = orientation(q1, q2, p1);
    i64 o4 = orientation(q1, q2, p2);

    if(((o1 > 0 && o2 < 0) || (o1 < 0 && o2 > 0)) && ((o3 > 0 && o4 < 0) || (o3 < 0 && o4 > 0))){
        return true;
    }

    return false;
}

bool inside_poly(std::vector<Point> const& points, Point const& a, Point const& b)
{
    i64 x0 = std::min(a.x, b.x);
    i64 y0 = std::min(a.y, b.y);
    i64 x1 = std::max(a.x, b.x);
    i64 y1 = std::max(a.y, b.y);

    Point r0 { x0, y0 };
    Point r1 { x1, y0 };
    Point r2 { x1, y1 };
    Point r3 { x0, y1 };

	// Check so that all corners are inside the polygon
    if(!point_in_poly(points, r0)) return false;
    if(!point_in_poly(points, r1)) return false;
    if(!point_in_poly(points, r2)) return false;
    if(!point_in_poly(points, r3)) return false;

	// Ensure that no "segments" intersect the square
    for(size_t i=0; i < points.size(); ++i)
    {
        Point p0 = points[i];
        Point p1 = points[(i+1) % points.size()];

        if(has_intersection(r0, r1, p0, p1)) return false;
        if(has_intersection(r1, r2, p0, p1)) return false;
        if(has_intersection(r2, r3, p0, p1)) return false;
        if(has_intersection(r3, r0, p0, p1)) return false;
    }

    return true;
}

}  // namespace 


int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	std::vector<Point> points;

	auto infile = std::ifstream("input.txt");
	std::string line;
	while (std::getline(infile, line)) {
		if (line.empty()) {
			break;
		}
		points.emplace_back();
		std::istringstream iss(line);
		iss >> points.back().x;
		iss.ignore(1);
		iss >> points.back().y;
	}
	infile.close();

	std::vector<Rectangle> rectangles;
	for (auto const& p : all_pairs(points.size())) {
		rectangles.emplace_back(points[p.first], points[p.second]);
	}

	std::ranges::sort(rectangles);
	std::cout << "Part 1: " << (rectangles | rv::reverse).front().area << std::endl;

	rectangles.clear();
	for (auto const& p : all_pairs(points.size())) {
		if (inside_poly(points, points[p.first], points[p.second])) {
			rectangles.emplace_back(points[p.first], points[p.second]);
		}
	}

	std::ranges::sort(rectangles);
	std::cout << "Part 2: " << (rectangles | rv::reverse).front().area << std::endl;
	return 0;
}
