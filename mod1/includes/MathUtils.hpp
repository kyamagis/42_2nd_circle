#ifndef MATHUTILS_HPP
# define MATHUTILS_HPP


#include "./Vec.hpp"
#include <cmath>

#define EPS 1e-6

int32_t	cross_product(const Vec &a, const Vec &b, const Vec &point);

uint32_t	max(uint32_t	aX, uint32_t bX);

void	assign_power_of_two(uint32_t &x, uint32_t &y, uint32_t &z);
#endif