#include <iostream>
#include <string.h>

#include "../includes/Utils.hpp"

int32_t	cross_product_2d(const Vec &a, const Vec &b, const Vec &point)
{
	int32_t	a_to_bx = b.x - a.x;
	int32_t	a_to_by = b.y - a.y;

	int32_t	b_to_px = point.x - b.x;
	int32_t b_to_py = point.y - b.y;

	return (a_to_bx * b_to_py) - (a_to_by * b_to_px);
}

void	assign_power_of_two(uint32_t &x, uint32_t &y, uint32_t &z)
{
	size_t	i = 1;
	for (; i <= x; ++i)
	{
		i *= 2;
	}
	x = i;
	y = i;
	z = i;
}

uint32_t	max(uint32_t	aX, uint32_t bX)
{
	if (aX < bX)
	{
		return bX;
	}
	return aX;
}

double	max(double	aX, double bX)
{
	if (aX < bX)
	{
		return bX;
	}
	return aX;
}

double	max_of_3_elm(double	aX, double bX, double cX)
{
	double	biggerX = max(aX, bX);

	if (biggerX < cX)
	{
		return cX;
	}
	return biggerX;
}

double	min(double	aX, double bX)
{
	if (aX < bX)
	{
		return aX;
	}
	return bX;
}

double	min_of_3_elm(double	aX, double bX, double cX)
{
	double	smallerX = min(aX, bX);

	if (smallerX < cX)
	{
		return smallerX;
	}
	return smallerX;
}

double	extend_map(const uint32_t mapSize)
{
	return mapSize + 2.0 * InvisibleSpace;
}

size_t	to_bucket_coor(const double coordinate)
{
	return size_t(coordinate / BUCKET_LENGTH);
}