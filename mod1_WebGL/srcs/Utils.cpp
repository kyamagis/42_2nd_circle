#include <iostream>
#include <string.h>
#include <fstream>
#include <cstdlib>

#include "../includes/Defines.hpp"
#include "../includes/Print.hpp"
#include "../includes/Utils.hpp"

int64_t	cross_product_2d(const Vec &a, const Vec &b, const Vec &point)
{
	const int64_t	a_to_bx = b.x - a.x;
	const int64_t	a_to_by = b.y - a.y;
	const int64_t	b_to_px = point.x - b.x;
	const int64_t	b_to_py = point.y - b.y;

	return (a_to_bx * b_to_py) - (a_to_by * b_to_px);
}

void	assign_power_of_two(uint32_t &x, uint32_t &y, uint32_t &z)
{
	size_t	i = 1;
	for (; i <= x - 1; ++i)
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
	const double	biggerX = max(aX, bX);
	return max(biggerX, cX);
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
	const double	smallerX = min(aX, bX);
	return min(smallerX, cX);
}


Vec	min(const Vec a, const Vec b)
{
	if (a.Magnitude3d() < b.Magnitude3d())
	{
		return a;
	}
	return b;
}

Vec	min_of_3_vec(const Vec a, const Vec b, const Vec c)
{
	const Vec	smallerVec = min(a, b);
	return min(smallerVec, c);
}

size_t	to_bucket_coor(const double coordinate)
{
	return size_t(coordinate / BUCKET_LENGTH);
}

Vec	gradation(const int64_t	maxHeight,
				const int64_t minHeight,
				const int64_t height)
{
	const double	midHeight = (maxHeight + minHeight) / 2.0;
	double	ratio;
	Vec		color;
	if (height <= midHeight)
	{
		ratio = (height - minHeight) / (midHeight - minHeight);
		color.x = 0.5f;
		color.y = ratio;
		color.z = 1.0f - ratio;
	}
	else if (height <= maxHeight)
	{
		ratio = (height - midHeight) / (maxHeight - midHeight);
		color.x = ratio;
		color.y = 1.0f - ratio;
		color.z = 0.0f;
	}
	return color;
}

void	line_gradation(const int64_t	maxHeight,
						const int64_t	minHeight,
						const double	midHeight, 
						const int64_t height)
{
	double	ratio;

	if (height <= midHeight)
	{
		ratio = (height - minHeight) / (midHeight - minHeight);
		glColor3f(0.0f, ratio, 1.0f - ratio);
	}
	else if (height <= maxHeight)
	{
		ratio = (height - midHeight) / (maxHeight - midHeight);
		glColor3f(ratio, 1.0f - ratio, 0.0f);
	}
}

double	calibrate_dist(const double dist)
{
	if (dist < 0.1)
	{
		return	0.1;
	}
	return	dist;
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
		Print::Err("calc_wall_weight");
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

	if (std::system(script) != 0)
	{
		Print::Err("std::system");
	}
	return weights;
}

Vec	move_vec_to_map_center(const Vec &vec, const Vec &halfMapSize, const double midHeight)
{
	return	Vec(vec.x - halfMapSize.x,
				vec.y - halfMapSize.y, 
				vec.z - midHeight / 2.0);
}

double trilinear_interpolation_dist(const Vec &pPos, 
									const size_t currentBX, const size_t currentBY, const size_t currentBZ, 
									const double *dists)
{
	const Vec	diagonalPoint(BUCKET_LENGTH);
	const Vec	calibratedPPos(pPos.x - currentBX * BUCKET_LENGTH, 
							   pPos.y - currentBY * BUCKET_LENGTH, 
							   pPos.z - currentBZ * BUCKET_LENGTH);

	const double r_00 = ((diagonalPoint.x - calibratedPPos.x) / (diagonalPoint.x)) * dists[e_000] + ((calibratedPPos.x) / (diagonalPoint.x)) * dists[e_100];
	const double r_01 = ((diagonalPoint.x - calibratedPPos.x) / (diagonalPoint.x)) * dists[e_010] + ((calibratedPPos.x) / (diagonalPoint.x)) * dists[e_110];
	const double r_10 = ((diagonalPoint.x - calibratedPPos.x) / (diagonalPoint.x)) * dists[e_001] + ((calibratedPPos.x) / (diagonalPoint.x)) * dists[e_101];
	const double r_11 = ((diagonalPoint.x - calibratedPPos.x) / (diagonalPoint.x)) * dists[e_011] + ((calibratedPPos.x) / (diagonalPoint.x)) * dists[e_111];

	double p_0 = ((diagonalPoint.y - calibratedPPos.y) / (diagonalPoint.y)) * r_00 + ((calibratedPPos.y) / (diagonalPoint.y)) * r_01;
	double p_1 = ((diagonalPoint.y - calibratedPPos.y) / (diagonalPoint.y)) * r_10 + ((calibratedPPos.y) / (diagonalPoint.y)) * r_11;

	return ((diagonalPoint.z - calibratedPPos.z) / (diagonalPoint.z)) * p_0 + ((calibratedPPos.z) / (diagonalPoint.z)) * p_1;
}

Vec	calc_n_vec(const double *distFromWalls)
{
	const double bl2 = 2.0 * BUCKET_LENGTH;
	Vec	nVec;

	nVec.x = (distFromWalls[e_100] - distFromWalls[e_000] + 
			  distFromWalls[e_110] - distFromWalls[e_010] + 
			  distFromWalls[e_101] - distFromWalls[e_001] + 
			  distFromWalls[e_111] - distFromWalls[e_011]) / (bl2);

	nVec.y = (distFromWalls[e_010] - distFromWalls[e_000] +
			  distFromWalls[e_110] - distFromWalls[e_100] +
			  distFromWalls[e_011] - distFromWalls[e_001] +
			  distFromWalls[e_111] - distFromWalls[e_101]) / (bl2);

	nVec.z = (distFromWalls[e_001] - distFromWalls[e_000] +
			  distFromWalls[e_101] - distFromWalls[e_100] +
			  distFromWalls[e_011] - distFromWalls[e_010] +
			  distFromWalls[e_111] - distFromWalls[e_110]) / (bl2);

	return nVec.Normalized3d();
}

void	add_bottom(std::deque<Triangle> &ts, 
					const Vec &mapSizeOrigin,
					const Vec &mapSizeExtended,
					const double extendSize)
{
	Vec	vertexA(0,0,0);
	Vec	vertexB(mapSizeExtended.x, 0, 0);
	Vec	vertexC(extendSize, extendSize, 0);

	ts.push_back({vertexA, vertexB, vertexC, false});
	ts.back().CalcNormalVector();
	ts.back().n.z = abs(ts.back().n.z);

	vertexA = Vec(mapSizeOrigin.x + extendSize, extendSize, 0);

	ts.push_back({vertexA, vertexB, vertexC, false});
	ts.back().CalcNormalVector();
	ts.back().n.z = abs(ts.back().n.z);

	vertexC = Vec(mapSizeExtended.x, mapSizeExtended.y, 0);

	ts.push_back({vertexA, vertexB, vertexC, false});
	ts.back().CalcNormalVector();
	ts.back().n.z = abs(ts.back().n.z);

	vertexB = Vec(mapSizeOrigin.x + extendSize, mapSizeOrigin.y + extendSize, 0);

	ts.push_back({vertexA, vertexB, vertexC, false});
	ts.back().CalcNormalVector();
	ts.back().n.z = abs(ts.back().n.z);

	vertexA = Vec(extendSize, mapSizeOrigin.y + extendSize, 0);

	ts.push_back({vertexA, vertexB, vertexC, false});
	ts.back().CalcNormalVector();
	ts.back().n.z = abs(ts.back().n.z);

	vertexB = Vec(0, mapSizeExtended.y, 0);

	ts.push_back({vertexA, vertexB, vertexC, false});
	ts.back().CalcNormalVector();
	ts.back().n.z = abs(ts.back().n.z);

	vertexC = Vec(extendSize, extendSize, 0);

	ts.push_back({vertexA, vertexB, vertexC, false});
	ts.back().CalcNormalVector();
	ts.back().n.z = abs(ts.back().n.z);

	vertexA = Vec(0, 0, 0);

	ts.push_back({vertexA, vertexB, vertexC, false});
	ts.back().CalcNormalVector();
	ts.back().n.z = abs(ts.back().n.z);
}

void	add_top(std::deque<Triangle> &ts, const Vec &mapSize)
{
	Vec	vertexA(mapSize.x, 0, mapSize.z);
	const Vec	vertexB(mapSize.x, mapSize.y, mapSize.z);
	const Vec	vertexC(0, 0, mapSize.z);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.z = -abs(ts.back().n.z);

	vertexA = Vec(0, mapSize.y, mapSize.z);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.z = -abs(ts.back().n.z);
}

static void	add_leftSide(std::deque<Triangle> &ts, const Vec &mapSize)
{
	Vec	vertexA(0, 0, 0);
	const Vec	vertexB(0, mapSize.y, 0);
	const Vec	vertexC(0, 0, mapSize.z);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.x = abs(ts.back().n.x);

	vertexA = Vec(0, mapSize.y, mapSize.z);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.x = abs(ts.back().n.x);
}

static void	add_rightSide(std::deque<Triangle> &ts, const Vec &mapSize)
{
	Vec	vertexA(mapSize.x, 0, mapSize.z);
	const Vec	vertexB(mapSize.x, 0, 0);
	const Vec	vertexC(mapSize.x, mapSize.y, mapSize.z);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.x = -abs(ts.back().n.x);

	vertexA = Vec(mapSize.x, mapSize.y, 0);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.x = -abs(ts.back().n.x);
}

static void	add_deepInTheFront(std::deque<Triangle> &ts, const Vec &mapSize)
{
	Vec	vertexA(0, 0, 0);
	const Vec	vertexB(mapSize.x, 0, 0);
	const Vec	vertexC(0, 0, mapSize.z);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.y = abs(ts.back().n.y);

	vertexA = Vec(mapSize.x, 0, mapSize.z);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.y = abs(ts.back().n.y);
}

static void	add_front(std::deque<Triangle> &ts, const Vec &mapSize)
{
	Vec	vertexA(0, mapSize.y, mapSize.z);
	const Vec	vertexB(mapSize.x, mapSize.y, mapSize.z);
	const Vec	vertexC(0, mapSize.y, 0);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.y = -abs(ts.back().n.y);

	vertexA = Vec(mapSize.x, mapSize.y, 0);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.y = -abs(ts.back().n.y);
}

void	add_cube(std::deque<Triangle> &ts, const Vec &mapSize)
{
	add_leftSide(ts, mapSize);
	add_rightSide(ts, mapSize);
	add_front(ts, mapSize);
	add_deepInTheFront(ts, mapSize);
}
