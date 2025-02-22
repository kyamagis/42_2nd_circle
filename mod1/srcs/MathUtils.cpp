#include <iostream>
#include <string.h>

#include "../includes/MathUtils.hpp"

int32_t	cross_product(const Vec &a, const Vec &b, const Vec &point)
{
	int32_t	a_to_bx = b.x - a.x;
	int32_t	a_to_by = b.y - a.y;

	int32_t	b_to_px = point.x - b.x;
	int32_t b_to_py = point.y - b.y;

	return (a_to_bx * b_to_py) - (a_to_by * b_to_px);
}

void	assign_power_of_two(uint32_t &x, uint32_t &y)
{
	size_t	i = 1;
	for (; i <= y; ++i)
	{
		i *= 2;
	}
	x = i;
	y = i;
}