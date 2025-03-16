#ifndef UTILS_HPP
# define UTILS_HPP

#include <cmath>

#include "./Graphic.hpp"
#include "./Vec.hpp"

int64_t	cross_product_2d(const Vec &a, const Vec &b, const Vec &point);

uint32_t	max(uint32_t	aX, uint32_t bX);
double	min(const double aX, const double bX);
double  max_of_3_elm(const double aX, const double bX, const double cX);
double	min_of_3_elm(const double aX, const double bX, const double cX);
void	assign_power_of_two(uint32_t &x, uint32_t &y, uint32_t &z);

size_t	to_bucket_coor(const double coordinate);

Vec		gradation(const int64_t	maxHeight,
					const int64_t	minHeight, 
					const int64_t height);
void	line_gradation(const int64_t	maxHeight,
						const int64_t	minHeight,
						const double	midHeight,  
						const int64_t height);

std::deque<Vec>	init_wall_weight(void);

Vec		move_vec_to_map_center(const Vec &vec, const Vec &halfMapSize, const double midHeight);

double trilinear_interpolation_dist(const Vec &pPos, const size_t currentBX,
								const size_t currentBY,
								const size_t currentBZ, 
								double dist_000, double dist_100, double dist_010, double dist_110, 
								double dist_001, double dist_101, double dist_011, double dist_111);

Vec trilinear_interpolation_nVec(const Vec &pPos, const size_t currentBX,
								const size_t currentBY,
								const size_t currentBZ, 
								Vec dist_000, Vec dist_100, Vec dist_010, Vec dist_110, 
								Vec dist_001, Vec dist_101, Vec dist_011, Vec dist_111);

Vec	calc_n_vec(const double *distFromWallSQs);

double	calibrate_dist(const double dist);

void	add_cube(std::deque<Triangle> &ts, const Vec &mapSize);

#endif