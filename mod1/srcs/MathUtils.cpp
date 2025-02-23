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

double	Max(double	aX, double bX)
{
	if (aX < bX)
	{
		return bX;
	}
	return aX;
}

double MaxOf3Elm(double	aX, double bX, double cX)
{
	double	biggerX = Max(aX, bX);

	if (biggerX < cX)
	{
		return cX;
	}
	return biggerX;
}

Vec	MaxEachCoordinateOfVertex(const Vec &a, 
							  const Vec &b, 
							  const Vec &c)
{
	Vec maxCoordinate;

	maxCoordinate.x = MaxOf3Elm(a.x, b.x, c.x) + E_RADIUS;
	maxCoordinate.y = MaxOf3Elm(a.y, b.y, c.y) + E_RADIUS;
	maxCoordinate.z = MaxOf3Elm(a.z, b.z, c.z) + E_RADIUS;

	return maxCoordinate;
}

// Vec	Min(const Vec &a, const Vec &b, const Vec &c)
// {
	
// }