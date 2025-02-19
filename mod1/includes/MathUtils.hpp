#ifndef MATHUTILS_HPP
# define MATHUTILS_HPP


#include "./Vec.hpp"
#include <cmath>

#define EPS 1e-6

int32_t	cross_product(const Vec &a, const Vec &b, const Vec &point);

double	magnitude_sq_2d(const Vec &a);
double	magnitude_2d(const Vec &a);


double	cos_angle(const Vec &a, const Vec &point, const Vec &b);

void	z_rotation(Vec &vertex, double rad, double x, double y);

void	x_rotation(Vec &vertex, double rad, double y, double z);

void	y_rotation(Vec &vertex, double rad, double x, double z);

void	assign_power_of_two(uint32_t &x, uint32_t &y);
#endif