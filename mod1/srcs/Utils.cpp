#include <iostream>
#include <string.h>
#include <fstream>
#include <cstdlib>

#include "../includes/Print.hpp"
#include "../includes/Utils.hpp"

int64_t	cross_product_2d(const Vec &a, const Vec &b, const Vec &point)
{
	const int64_t	a_to_bx = b.x - a.x;
	const int64_t	a_to_by = b.y - a.y;

	const int64_t	b_to_px = point.x - b.x;
	const int64_t b_to_py = point.y - b.y;

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

Vec	move_vec_to_map_center(const Vec &vec, const Vec &halfMapSize, const double midHeight)
{
	return	Vec(vec.x - halfMapSize.x,
				vec.y - halfMapSize.y, 
				vec.z - midHeight / 2.0);
}


// 三線形補間関数
double trilinear_interpolation(const Vec &pPos, const size_t currentBX,
								const size_t currentBY,
								const size_t currentBZ, 
							double dist_000, double dist_100, double dist_010, double dist_110, 
							double dist_001, double dist_101, double dist_011, double dist_111)
{
	// const Vec	origin;
	const Vec	diagonalPoint(BUCKET_LENGTH);
	const Vec	calibratedPPos(pPos.x - currentBX * BUCKET_LENGTH, 
							   pPos.y - currentBY * BUCKET_LENGTH, 
							   pPos.z - currentBZ * BUCKET_LENGTH);

	const double R00 = ((diagonalPoint.x - calibratedPPos.x) / (diagonalPoint.x)) * dist_000 + ((calibratedPPos.x) / (diagonalPoint.x)) * dist_100;
	const double R01 = ((diagonalPoint.x - calibratedPPos.x) / (diagonalPoint.x)) * dist_010 + ((calibratedPPos.x) / (diagonalPoint.x)) * dist_110;
	const double R10 = ((diagonalPoint.x - calibratedPPos.x) / (diagonalPoint.x)) * dist_001 + ((calibratedPPos.x) / (diagonalPoint.x)) * dist_101;
	const double R11 = ((diagonalPoint.x - calibratedPPos.x) / (diagonalPoint.x)) * dist_011 + ((calibratedPPos.x) / (diagonalPoint.x)) * dist_111;

	// Y方向補間
	double P0 = ((diagonalPoint.y - calibratedPPos.y) / (diagonalPoint.y)) * R00 + ((calibratedPPos.y) / (diagonalPoint.y)) * R01;
	double P1 = ((diagonalPoint.y - calibratedPPos.y) / (diagonalPoint.y)) * R10 + ((calibratedPPos.y) / (diagonalPoint.y)) * R11;

	// Z方向補間
	return ((diagonalPoint.z - calibratedPPos.z) / (diagonalPoint.z)) * P0 + ((calibratedPPos.z) / (diagonalPoint.z)) * P1;
}

static void	add_bottom(std::deque<Triangle> &ts, const Vec &mapSize)
{
	Vec	vertexA(0,0,0);
	Vec	vertexB(0, mapSize.y - 1, 0);
	Vec	vertexC(mapSize.x - 1, 0, 0);

	ts.push_back({vertexA, vertexB, vertexC, false});
	ts.back().CalcNormalVector();
	ts.back().n.z = abs(ts.back().n.z);

	vertexA = Vec(mapSize.x - 1, mapSize.y - 1, 0);

	ts.push_back({vertexA, vertexB, vertexC, false});
	ts.back().CalcNormalVector();
	ts.back().n.z = abs(ts.back().n.z);
}

static void	add_top(std::deque<Triangle> &ts, const Vec &mapSize)
{
	Vec	vertexA(0, 0, mapSize.z - 1);
	Vec	vertexB(0, mapSize.y - 1, mapSize.z - 1);
	Vec	vertexC(mapSize.x - 1, 0, mapSize.z - 1);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.z = -abs(ts.back().n.z);

	vertexA = Vec(mapSize.x - 1, mapSize.y - 1, mapSize.z - 1);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.z = -abs(ts.back().n.z);
}

static void	add_leftSide(std::deque<Triangle> &ts, const Vec &mapSize)
{
	Vec	vertexA(0, 0, 0);
	Vec	vertexB(0, mapSize.y - 1, 0);
	Vec	vertexC(0, 0, mapSize.z - 1);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.x = abs(ts.back().n.x);

	vertexA = Vec(0, mapSize.y - 1, mapSize.z - 1);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.x = abs(ts.back().n.x);
}

static void	add_rightSide(std::deque<Triangle> &ts, const Vec &mapSize)
{
	Vec	vertexA(mapSize.x - 1, 0, 0);
	Vec	vertexB(mapSize.x - 1, mapSize.y - 1, 0);
	Vec	vertexC(mapSize.x - 1, 0, mapSize.z - 1);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.x = -abs(ts.back().n.x);

	vertexA = Vec(mapSize.x - 1, mapSize.y - 1, mapSize.z - 1);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.x = -abs(ts.back().n.x);
}

static void	add_deepInTheFront(std::deque<Triangle> &ts, const Vec &mapSize)
{
	Vec	vertexA(0, 0, 0);
	Vec	vertexB(mapSize.x - 1, 0, 0);
	Vec	vertexC(0, 0, mapSize.z - 1);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.y = abs(ts.back().n.y);

	vertexA = Vec(mapSize.x - 1, 0, mapSize.z - 1);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.y = abs(ts.back().n.y);
}

static void	add_front(std::deque<Triangle> &ts, const Vec &mapSize)
{
	Vec	vertexA(0, mapSize.y - 1, 0);
	Vec	vertexB(mapSize.x - 1, mapSize.y - 1, 0);
	Vec	vertexC(0, mapSize.y - 1, mapSize.z - 1);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.y = -abs(ts.back().n.y);

	vertexA = Vec(mapSize.x - 1, mapSize.y - 1, mapSize.z - 1);

	ts.push_back({vertexA, vertexB, vertexC, false, false});
	ts.back().CalcNormalVector();
	ts.back().n.y = -abs(ts.back().n.y);
}

void	add_cube(std::deque<Triangle> &ts, const Vec &mapSize)
{
	add_bottom(ts, mapSize);
	add_top(ts, mapSize);
	add_leftSide(ts, mapSize);
	add_rightSide(ts, mapSize);
	add_front(ts, mapSize);
	add_deepInTheFront(ts, mapSize);
}
