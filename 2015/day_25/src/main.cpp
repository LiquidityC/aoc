#include <iostream>
#include <cstdint>
#include <cassert>

typedef uint64_t u64;

u64 pairing(u64 x, u64 y)
{
	u64 x0 = ((x * (1+x)) / 2);
	u64 y0 = (((y-2) * (y-1)) / 2);
	u64 xy = x0 + (x*(y-1)) + y0;
	return xy;
}

u64 gencode(u64 x, u64 y)
{
	u64 p = pairing(x, y);
	u64 sum = 20151125;

	for (size_t i = 2; i <= p; ++i) {
		sum = (sum * 252533) % 33554393;
	}

	return sum;
}

int main()
{
	std::cout << "Part 1: " << gencode(3019, 3010) << std::endl;
}
