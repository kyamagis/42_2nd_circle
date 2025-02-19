// Inverse Distance Weighting

#include <iostream>
#include <string.h>

#include "../includes/MathUtils.hpp"


#define	POINTS_NUM 20
#define DIMENSION 3


int32_t	cross_product(const Vec &a, const Vec &b, const Vec &point)
{
	int32_t	a_to_bx = b.x - a.x;
	int32_t	a_to_by = b.y - a.y;

	int32_t	b_to_px = point.x - b.x;
	int32_t b_to_py = point.y - b.y;

	return (a_to_bx * b_to_py) - (a_to_by * b_to_px);
}

double	dot_product_2d(const Vec &a, const Vec &b)
{
	return (a.x * b.x) + (a.y * b.y);
}


double	dot_product_3d(const Vec &a, const Vec &b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

double	magnitude_sq_2d(const Vec &a)
{
	return a.x * a.x + a.y * a.y;
}

double	magnitude_2d(const Vec &a)
{
	return sqrt(magnitude_sq_2d(a));
}


double	cos_angle(const Vec &a, const Vec &point, const Vec &b)
{
	return dot_product_2d(a - point, b - point) 
			/ (magnitude_2d(a - point) * magnitude_2d(b - point));
}

void	z_rotation(Vec &vertex, double rad, double x, double y)
{
	vertex.x = x * cos(rad) - y * sin(rad);
	vertex.y = x * sin(rad) + y * cos(rad);
}

void	x_rotation(Vec &vertex, double rad, double y, double z)
{
	vertex.y =  y * cos(rad) - z * sin(rad);
	vertex.z =  y * sin(rad) + z * cos(rad);
}

void	y_rotation(Vec &vertex, double rad, double x, double z)
{
	vertex.x =  x * cos(rad) + z * sin(rad);
	vertex.z = -x * sin(rad) + z * cos(rad);
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