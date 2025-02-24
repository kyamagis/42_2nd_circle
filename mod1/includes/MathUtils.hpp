#ifndef MATHUTILS_HPP
# define MATHUTILS_HPP


#include "./Vec.hpp"
#include <cmath>

#define EPS 1e-6

int32_t	cross_product_2d(const Vec &a, const Vec &b, const Vec &point);

uint32_t	max(uint32_t	aX, uint32_t bX);
double  max_of_3_elm(double	aX, double bX, double cX);
double	min_of_3_elm(double	aX, double bX, double cX);
void	assign_power_of_two(uint32_t &x, uint32_t &y, uint32_t &z);
#endif