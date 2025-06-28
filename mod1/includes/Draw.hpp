#ifndef DRAW_HPP
# define DRAW_HPP

#include "./Triangle.hpp"
#include "./Defines.hpp"
#include "./Structs.hpp"
#include "./BucketsController.hpp"
#include "./Defines.hpp"

void	draw_triangle(const t_data &data, const Triangle &triangle);

void	draw_particles(const t_data &data);

void	draw_circle_2d(const t_data &data, 
                        const Circle &circle,
						const size_t num_segments);

#endif