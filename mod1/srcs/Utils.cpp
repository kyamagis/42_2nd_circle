#include <iostream>
#include <string.h>
#include <fstream>
#include <cstdlib>

#include "../includes/Print.hpp"
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

double	max(const double aX, const double bX)
{
	if (aX < bX)
	{
		return bX;
	}
	return aX;
}

double	max_of_3_elm(const double aX, const double bX, const double cX)
{
	double	biggerX = max(aX, bX);

	if (biggerX < cX)
	{
		return cX;
	}
	return biggerX;
}

double	min(const double aX, const double bX)
{
	if (aX < bX)
	{
		return aX;
	}
	return bX;
}

double	min_of_3_elm(const double aX, const double bX, const double cX)
{
	double	smallerX = min(aX, bX);

	if (smallerX < cX)
	{
		return smallerX;
	}
	return cX;
}

double	extend_map(const uint32_t mapSize)
{
	return mapSize + 2.0 * InvisibleSpace;
}

size_t	to_bucket_coor(const double coordinate)
{
	return size_t(coordinate / BUCKET_LENGTH);
}

void	line_gradation(const int64_t	maxHeight,
						const int64_t	minHeight,
						const double	midHeight, 
						const int64_t height)
{
	// double	ratio, ratio2;
	double	ratio;

	if (height <= midHeight)
	{
		ratio = height - minHeight / (midHeight - minHeight);
		glColor3f(0.0, ratio, 1 - ratio);
	}
	else
	{
		ratio = height - midHeight / (maxHeight - midHeight);
		glColor3f(ratio, 1 - ratio, 0.0);
	}
}

static void	init_ps(std::deque<Particle> &ps)
{
	double	x;
	double	y;
	double	z;

	for (int32_t idxX = -4; idxX <= 0; ++idxX) {
	for (int32_t idxY = -4; idxY <= 0; ++idxY) {
	for (int32_t idxZ = -4; idxZ <= 0; ++idxZ) {
		x =  I_DISTANCE * double(idxX);
		y =  I_DISTANCE * double(idxY);
		z =  I_DISTANCE * double(idxZ);
		ps.push_back(Particle(x, y, z));
	}}}
}

static void	calc_wall_weight(std::deque<Vec> &weights, std::deque<Particle> &ps)
{
	Particle	point(0,RADIUS,0);
	double		distance;
	double		distanceSQ;
	std::ofstream outputfile("WallWeight.csv");
	if (outputfile.fail())
	{
		Print::Err("_InitWallWeight");
		std::exit(EXIT_FAILURE);
	}
	std::deque<Particle>::iterator	it;
	const double	diff = BUCKET_LENGTH / 40;
	double			weight;

	while (0 < ps.size())
	{
		it = ps.begin();
		weight = 0.0;
		while (it != ps.end())
		{
			distanceSQ = (it)->center.MagnitudeSQ3d(point.center);
			if (distanceSQ <= E_RADIUS_SQ)
			{
				distance = sqrt(distanceSQ);
				weight += WEIGHT(distance);
				++it;
			}
			else
			{
				it = ps.erase(it);
			}
		}
		// std::cout <<  ps.size() << std::endl;
		outputfile << point.center.y - RADIUS << ", " << weight << std::endl;
		weights.push_back(Vec(point.center.y - RADIUS, weight, 0));
		point.center.y += diff;
	}
	outputfile.close();
}

std::deque<Vec>	init_wall_weight(void)
{
	std::deque<Particle>	ps;
	std::deque<Vec>			weights;

	init_ps(ps);
	calc_wall_weight(weights, ps);

	const char* script = "python3 ./srcs/WallWeight.py";

    // Pythonスクリプトを実行
	if (std::system(script) != 0)
	{
		Print::Err("std::system");
	}
	return weights;
}

void	move_vec_to_map_center(Vec &vec, const Vec &halfMapSize, const double midHeight)
{
	vec.x -= halfMapSize.x;
	vec.y -= halfMapSize.y;
	vec.z -= midHeight / 2.0;
}

void	rotation_ts(std::deque<Triangle> &ts)
{
	for (size_t	i = 0; i < ts.size(); ++i)
	{
		ts[i].Rotation();
	}
}