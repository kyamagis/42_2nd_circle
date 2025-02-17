#ifndef MATHUTILS_HPP
# define MATHUTILS_HPP


#include "./Vec.hpp"
#include <cmath>

int32_t	cross_product(const Vec &a, const Vec &b, const Vec &point);

double	magnitude_sq(double x0, double y0, double x1, double y1);
double	magnitude(double x0, double y0, double x1, double y1);

double	cos_angle(const Vec &a, const Vec &point, const Vec &b);

void	z_rotation(Vec &vertex, double rad, double x, double y);

void	x_rotation(Vec &vertex, double rad, double y, double z);

void	y_rotation(Vec &vertex, double rad, double x, double z);
#endif