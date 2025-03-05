#ifndef UTILS_HPP
# define UTILS_HPP

#include <cmath>

#include "./Graphic.hpp"
#include "./Vec.hpp"

#define EPS 1e-6

int32_t	cross_product_2d(const Vec &a, const Vec &b, const Vec &point);

uint32_t	max(uint32_t	aX, uint32_t bX);
double	min(const double aX, const double bX);
double  max_of_3_elm(const double aX, const double bX, const double cX);
double	min_of_3_elm(const double aX, const double bX, const double cX);
void	assign_power_of_two(uint32_t &x, uint32_t &y, uint32_t &z);

double	extend_map(const uint32_t mapSize);
size_t	to_bucket_coor(const double coordinate);

void	line_gradation(const int64_t	maxHeight,
						const int64_t	minHeight,
						const double	midHeight,  
						const int64_t height);

std::deque<Vec>	init_wall_weight(void);

#endif