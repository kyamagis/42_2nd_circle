#include "../includes/BucketsController.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Defines.hpp"
#include "../includes/Graphic.hpp"
#include "../includes/Print.hpp"

// BC::BC()
// {
	
// }

BC::BC(const Vec &visibleMapSize_,
	   const Vec &totalMapSize_)
	   :_visibleMapSize(visibleMapSize_ + BUCKET_LENGTH),
	    _totalMapSize(totalMapSize_ + BUCKET_LENGTH),
		_weights(init_wall_weight()),
		_bucketLast(NULL),
		bc_bucketLength(BUCKET_LENGTH),
		bc_radius_effective(E_RADIUS),
		bc_radius_effectiveSQ(E_RADIUS_SQ),
		bucketRow(to_bucket_coor(this->_totalMapSize.x) + 1),
		bucketColumn(to_bucket_coor(this->_totalMapSize.y) + 1),
		bucketDepth(to_bucket_coor(this->_totalMapSize.z) + 1),
		_columnMultiplDepth(this->bucketColumn * this->bucketDepth),
		numOfBuckets(this->bucketRow * this->bucketColumn * this->bucketDepth),
		buckets(NULL),
		particleNextIdxs(NULL)
{

}

BC::~BC()
{
	if (this->buckets != NULL)
	{
		delete [] this->buckets;
	}
	if (this->_bucketLast != NULL)
	{
		delete [] this->_bucketLast;
	}
	if (this->particleNextIdxs != NULL)
	{
		delete [] this->particleNextIdxs;
	}
}

void	BC::_UpdateParticlesInBuckets(const std::deque<Particle> &ps)
{
	size_t	bucketX;
	size_t	bucketY;
	size_t	bucketZ;
	size_t	bucketIdx;
	size_t	nextIdx;

	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		if (ps[i].validFlag == false)
		{
			continue;
		}
		bucketX = size_t(ps[i].center.x / this->bc_bucketLength);
		bucketY = size_t(ps[i].center.y / this->bc_bucketLength);
		bucketZ = size_t(ps[i].center.z / this->bc_bucketLength);

		bucketIdx = this->BC_CalcBucketIdx(bucketX, bucketY, bucketZ);
		nextIdx = this->_bucketLast[bucketIdx].firstPrtIdx;
		this->_bucketLast[bucketIdx].firstPrtIdx = i;
		if (nextIdx == UINT64_MAX)
		{
			this->buckets[bucketIdx].firstPrtIdx = i;
		}
		else
		{
			this->particleNextIdxs[nextIdx] = i;
		}
	}
}

void	BC::_CalcBucketsPos(const size_t i)
{
	size_t	bucketXY;

	this->buckets[i].bucketZ = i / this->_columnMultiplDepth;
	this->buckets[i].position.z = this->bc_bucketLength * this->buckets[i].bucketZ;

	bucketXY = i % this->_columnMultiplDepth;	
	this->buckets[i].bucketY = bucketXY / this->bucketColumn;
	this->buckets[i].position.y = this->bc_bucketLength * this->buckets[i].bucketY;
	
	this->buckets[i].bucketX = bucketXY % this->bucketColumn;
	this->buckets[i].position.x = this->bc_bucketLength * this->buckets[i].bucketX;

	this->buckets[i].center += this->buckets[i].position + this->bc_bucketLength / 2.0;
}



Vec	BC::_MaxEachCoordinateOfVertex(const Vec &a, 
									const Vec &b,
									const Vec &c)
{
	Vec maxCoordinate;

	maxCoordinate.x = max_of_3_elm(a.x, b.x, c.x);
	maxCoordinate.y = max_of_3_elm(a.y, b.y, c.y);
	maxCoordinate.z = max_of_3_elm(a.z, b.z, c.z);

	if (maxCoordinate.x + this->bc_radius_effective < this->_totalMapSize.x)
	{
		maxCoordinate.x += this->bc_radius_effective;
	}
	if (maxCoordinate.y + this->bc_radius_effective < this->_totalMapSize.y)
	{
		maxCoordinate.y += this->bc_radius_effective;
	}
	if (maxCoordinate.z + this->bc_radius_effective< this->_totalMapSize.z)
	{
		maxCoordinate.z += this->bc_radius_effective;
	}

	return maxCoordinate;
}

Vec	BC::_MinEachCoordinateOfVertex(const Vec &a, 
									const Vec &b,
									const Vec &c)
{
	Vec minCoordinate;

	minCoordinate.x = min_of_3_elm(a.x, b.x, c.x);
	minCoordinate.y = min_of_3_elm(a.y, b.y, c.y);
	minCoordinate.z = min_of_3_elm(a.z, b.z, c.z);

	if (this->bc_radius_effective < minCoordinate.x)
	{
		minCoordinate.x -= this->bc_radius_effective;
	}
	if (this->bc_radius_effective < minCoordinate.y)
	{
		minCoordinate.y -= this->bc_radius_effective;
	}
	if (this->bc_radius_effective < minCoordinate.z)
	{
		minCoordinate.z -= this->bc_radius_effective;
	}

	return minCoordinate;
}

double	BC::_CalcShortestDistanceFromVertexSQ(const Triangle &t, 
											const size_t bucketIdx)
{
	double	disFromVertexA = t.a.MagnitudeSQ3d(this->buckets[bucketIdx].position);
	double	disFromVertexB = t.b.MagnitudeSQ3d(this->buckets[bucketIdx].position);
	double	disFromVertexC = t.c.MagnitudeSQ3d(this->buckets[bucketIdx].position);

	return min_of_3_elm(disFromVertexA, disFromVertexB, disFromVertexC);
}

double	BC::_CalcDistanceFromSideSQ(const Vec &a, 
								  const Vec &b, 
								  const size_t bucketIdx)
{
	const Vec	abVector = b - a;
	const Vec	aCenterVector = this->buckets[bucketIdx].position - a;

	const double	t = abVector.DotProduct3d(aCenterVector) / 
						abVector.MagnitudeSQ3d();

	if (0.0 < t && t < 1.0)
	{
		const Vec	projectivePoint = a + (abVector * t);

	
		return (this->buckets[bucketIdx].position - projectivePoint).MagnitudeSQ3d();
	}
		
	return 2.0 * this->bc_bucketLength * 2.0 * this->bc_bucketLength;
}

double	BC::_CalcShortestDistanceFromSideSQ(const Triangle &t, const size_t bucketIdx)
{
	const double	disFromSideAB = this->_CalcDistanceFromSideSQ(t.a, t.b, bucketIdx);
	const double	disFromSideBC = this->_CalcDistanceFromSideSQ(t.b, t.c, bucketIdx);
	const double	disFromSideCA = this->_CalcDistanceFromSideSQ(t.c, t.a, bucketIdx);

	return min_of_3_elm(disFromSideAB, disFromSideBC, disFromSideCA);
}

double	BC::_CalcDistanceFromTriangleSQ(const Triangle &t, const size_t bucketIdx)
{
	const Vec		apVec =  this->buckets[bucketIdx].position - t.a;
	const double	coefficient = apVec.DotProduct3d(t.n) / t.n.MagnitudeSQ3d();
	const Vec 		orientVec = t.n * coefficient;
	const Vec		projectivePoint = this->buckets[bucketIdx].position - orientVec;
	
	if (t.InternalAndExternalJudgments3d(projectivePoint))
	{
		return orientVec.MagnitudeSQ3d();
	}
	return 2.0 * this->bc_bucketLength * 2.0 * this->bc_bucketLength;
}

double	BC::_CalcShortestDistanceSQ(const Triangle &t, const size_t bucketIdx)
{
	// const double	distFromVertex   = this->_CalcShortestDistanceFromVertexSQ(t, bucketIdx);
	// return distFromVertex;

	// const double	distFromVertex   = this->_CalcShortestDistanceFromVertexSQ(t, bucketIdx);
	// const double	distFromSide 	= this->_CalcShortestDistanceFromSideSQ(t,bucketIdx);
	// return min(distFromVertex, distFromSide);

	const double	distFromVertex   = this->_CalcShortestDistanceFromVertexSQ(t, bucketIdx);
	const double	distFromSide 	 = this->_CalcShortestDistanceFromSideSQ(t,bucketIdx);
	const double	distFromTriangle = this->_CalcDistanceFromTriangleSQ(t, bucketIdx);
	const double	shortestDistSQ   = min_of_3_elm(distFromVertex, distFromSide, distFromTriangle);

	return shortestDistSQ;
}

void	BC::_CalcDistanceFromWallSQ(const Triangle &t)
{
	const Vec	maxCrd = this->_MaxEachCoordinateOfVertex(t.a, t.b, t.c);
	const Vec	minCrd = this->_MinEachCoordinateOfVertex(t.a, t.b, t.c);

	size_t	bucketX = size_t(minCrd.x / this->bc_bucketLength);
	size_t	bucketY = size_t(minCrd.y / this->bc_bucketLength);
	size_t	bucketZ = size_t(minCrd.z / this->bc_bucketLength);
	size_t	bucketIdx;

	double	shortestDistSQ;
	
	for (double	i = minCrd.x; size_t(i) <= size_t(maxCrd.x);) {
	for (double	j = minCrd.y; size_t(j) <= size_t(maxCrd.y);) {
	for (double	k = minCrd.z; size_t(k) <= size_t(maxCrd.z);) {
				bucketIdx      = this->BC_CalcBucketIdx(bucketX, bucketY, bucketZ);
				shortestDistSQ = this->_CalcShortestDistanceSQ(t, bucketIdx);
				if (shortestDistSQ == this->buckets[bucketIdx].distFromWallSQ)
				{
					this->buckets[bucketIdx].n += t.n;
				}
				else if (shortestDistSQ < this->buckets[bucketIdx].distFromWallSQ)
				{
					this->buckets[bucketIdx].n = t.n;
					this->buckets[bucketIdx].distFromWallSQ = shortestDistSQ;
				}
				// if (this->buckets[bucketIdx].bucketX == 8 &&
				// 	this->buckets[bucketIdx].bucketY == 8 &&
				// 	this->buckets[bucketIdx].bucketZ)
				// {
				// 	Print::OutWords(this->buckets[bucketIdx].n, t.n);
				// }
				k += this->bc_bucketLength;
				bucketZ = k / this->bc_bucketLength;
			}
			bucketZ = size_t(minCrd.z / this->bc_bucketLength);
			j += this->bc_bucketLength;
			bucketY = j / this->bc_bucketLength;
		}
		bucketY = size_t(minCrd.y / this->bc_bucketLength);
		i += this->bc_bucketLength;
		bucketX = i / this->bc_bucketLength;
	}
}

double	BC::_GetDistFromWallSQ(const size_t currentBX,
						 	   const size_t currentBY,
						 	   const size_t currentBZ)
{
	const size_t	neighborBIdx = this->BC_CalcBucketIdx(currentBX, currentBY, currentBZ);
	return this->buckets[neighborBIdx].distFromWallSQ;
}

double	BC::_SearchNeighborBDistFromWallSQ(size_t currentBX,
										   size_t currentBY,
										   size_t currentBZ,
										   const unsigned char cmp)
{
	switch (cmp)
	{
		case 'x':
			currentBX += 1;
			break;
		case 'y':
			currentBY += 1;
			break;
		case 'z':
			currentBZ += 1;
			break;
		default:
		break;
	}
	return this->_GetDistFromWallSQ(currentBX, currentBY, currentBZ);
}

bool	BC::_StoreEachCmpOfNeighborBDistFromWallSQ(const size_t currentBX,
											const size_t currentBY,
											const size_t currentBZ,
											const unsigned char cmp,
											double &neighborBDistFromWallSQ)
{
	size_t	currentBcmp;
	size_t	maxRCD;

	switch (cmp)
	{
		case 'x':
			currentBcmp = currentBX;
			maxRCD = this->bucketRow;
			break;
		case 'y':
			currentBcmp = currentBY;
			maxRCD = this->bucketColumn;
			break;
		case 'z':
			currentBcmp = currentBZ;
			maxRCD = this->bucketDepth;
			break;
		default:
			return false;
	}

	if (currentBcmp + 1 < maxRCD)
	{
		neighborBDistFromWallSQ = 
		this->_SearchNeighborBDistFromWallSQ(currentBX, currentBY, currentBZ,
											  cmp);
	}
	else
	{
		neighborBDistFromWallSQ = this->bc_radius_effectiveSQ + EPS;
	}

	return currentBcmp + 1 < maxRCD;
}

void	BC::BC_MakeBuckets(const std::deque<Particle> &ps)
{
	// Print::OutWords("BC_MakeBuckets", this->bucketRow, this->bucketColumn, this->bucketDepth);
	this->buckets           = new t_bucket[this->numOfBuckets];
	this->_bucketLast       = new t_bucket[this->numOfBuckets];
	this->particleNextIdxs  = new size_t[NUM_OF_PARTICLES];

	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		this->buckets[i].firstPrtIdx     = UINT64_MAX;
		this->buckets[i].distFromWallSQ  = this->bc_radius_effectiveSQ + EPS;
		// this->buckets[i].bucketIdx       = i;
		this->_bucketLast[i].firstPrtIdx = UINT64_MAX;

		this->_CalcBucketsPos(i);
	
		if (i < NUM_OF_PARTICLES)
		{
			this->particleNextIdxs[i] = UINT64_MAX;
		}
	}
	this->_UpdateParticlesInBuckets(ps);
}

void	BC::BC_UpdateBuckets(const std::deque<Particle> &ps)
{
	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		this->buckets[i].firstPrtIdx     = UINT64_MAX;
		this->_bucketLast[i].firstPrtIdx = UINT64_MAX;		
		if (i < NUM_OF_PARTICLES)
		{
			this->particleNextIdxs[i] = UINT64_MAX;
		}
	}
	this->_UpdateParticlesInBuckets(ps);
}

void	BC::BC_CalcAllDistanceFromWallSQ(const std::deque<Triangle>	&ts)
{
	for (size_t	i = 0; i < ts.size(); ++i)
	{
		this->_CalcDistanceFromWallSQ(ts[i]);
	}
	double	nVecMagnitude;
	double	distFromWallSQs[8];

	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		nVecMagnitude = this->buckets[i].n.Magnitude3d();
		if (EPS < nVecMagnitude)
		{
			this->buckets[i].n /= this->buckets[i].n.Magnitude3d();
		}
		this->BC_InterpolateDistFromWallSQ(this->buckets[i].center, 
			this->buckets[i].bucketX, this->buckets[i].bucketY, this->buckets[i].bucketZ,
			distFromWallSQs);
		this->buckets[i].nInterpolation = calc_n_vec(distFromWallSQs);
	}
}

size_t	BC::BC_CalcBucketIdx(size_t bucketX, size_t bucketY, size_t bucketZ)
{
	return bucketX + 
		   this->bucketColumn * bucketY + 
		   this->_columnMultiplDepth * bucketZ;
}

size_t	BC::BC_CalcBucketIdx(const Vec &v)
{
	return this->BC_CalcBucketIdx(v.x, v.y, v.z);
}

bool	BC::BC_IsOutOfWallWeightRange(const double disFromWall)
{
	return this->_weights.back().x < disFromWall + EPS;
}

double	BC::BC_InterpolateWallWeight(const double interpolatedDist)
{
	Vec nextWeight;

	for (size_t	i = 0; i + 1 < this->_weights.size(); ++i)
	{
		nextWeight = this->_weights[i + 1];
		if (interpolatedDist <= nextWeight.x)
		{
			return this->_weights[i].Interpolate2d(nextWeight, interpolatedDist);
		}
	}
	Print::Err("BC_InterpolateWallWeight: weight");
	return 0.0;
}

double	BC::BC_InterpolateDistFromWallSQ(const Vec	 &pPos,
										const size_t currentBX,
										const size_t currentBY,
										const size_t currentBZ)
{
	const size_t	distFromWallSQ_000 = this->_GetDistFromWallSQ(currentBX, currentBY, currentBZ);
	double distFromWallSQ_100;
	double distFromWallSQ_110 = -1.0;
	double distFromWallSQ_101 = -1.0;
	double distFromWallSQ_111 = -1.0;

	double distFromWallSQ_010;
	// double distFromWallSQ_110;
	double distFromWallSQ_011 = -1.0;
	// double distFromWallSQ_111;

	double distFromWallSQ_001;
	// double distFromWallSQ_101;
	// double distFromWallSQ_011;
	// double distFromWallSQ_111;

	if (!this->_StoreEachCmpOfNeighborBDistFromWallSQ(currentBX, currentBY, currentBZ,
														'x', distFromWallSQ_100))
	{
		distFromWallSQ_110 = this->bc_radius_effectiveSQ + EPS;
		distFromWallSQ_101 = this->bc_radius_effectiveSQ + EPS;
		distFromWallSQ_111 = this->bc_radius_effectiveSQ + EPS;
	}

	if (!this->_StoreEachCmpOfNeighborBDistFromWallSQ(currentBX, currentBY, currentBZ,
														'y', distFromWallSQ_010))
	{
		distFromWallSQ_110 = this->bc_radius_effectiveSQ + EPS;
		distFromWallSQ_011 = this->bc_radius_effectiveSQ + EPS;
		distFromWallSQ_111 = this->bc_radius_effectiveSQ + EPS;
	}
	if (!this->_StoreEachCmpOfNeighborBDistFromWallSQ(currentBX, currentBY, currentBZ,
														'z', distFromWallSQ_001))
	{
		distFromWallSQ_101 = this->bc_radius_effectiveSQ + EPS;
		distFromWallSQ_011 = this->bc_radius_effectiveSQ + EPS;
		distFromWallSQ_111 = this->bc_radius_effectiveSQ + EPS;
	}
	if (distFromWallSQ_110 == -1.0)
	{
		distFromWallSQ_110 = this->_GetDistFromWallSQ(currentBX + 1, currentBY + 1, currentBZ);
	}
	if (distFromWallSQ_101 == -1.0)
	{
		distFromWallSQ_101 = this->_GetDistFromWallSQ(currentBX + 1, currentBY, currentBZ + 1);
	}
	if (distFromWallSQ_011 == -1.0)
	{
		distFromWallSQ_011 = this->_GetDistFromWallSQ(currentBX, currentBY + 1, currentBZ + 1);
	}
	if (distFromWallSQ_111 == -1.0)
	{
		distFromWallSQ_111 = this->_GetDistFromWallSQ(currentBX + 1, currentBY + 1, currentBZ + 1);
	}
	return	trilinear_interpolation_dist(pPos, currentBX, currentBY, currentBZ, 
									distFromWallSQ_000, distFromWallSQ_100, distFromWallSQ_010, distFromWallSQ_110, 
									distFromWallSQ_001, distFromWallSQ_101, distFromWallSQ_011, distFromWallSQ_111);
}

double	BC::BC_InterpolateDistFromWallSQ(const Vec	 &pPos,
										const size_t currentBX,
										const size_t currentBY,
										const size_t currentBZ,
										double *distFromWallSQs)
{
	distFromWallSQs[e_000] = this->_GetDistFromWallSQ(currentBX, currentBY, currentBZ);
	distFromWallSQs[e_110] = -1.0;
	distFromWallSQs[e_101] = -1.0;
	distFromWallSQs[e_111] = -1.0;
	distFromWallSQs[e_011] = -1.0;

	if (!this->_StoreEachCmpOfNeighborBDistFromWallSQ(currentBX, currentBY, currentBZ,
														'x', distFromWallSQs[e_100]))
	{
		distFromWallSQs[e_110] = this->bc_radius_effectiveSQ + EPS;
		distFromWallSQs[e_101] = this->bc_radius_effectiveSQ + EPS;
		distFromWallSQs[e_111] = this->bc_radius_effectiveSQ + EPS;
	}

	if (!this->_StoreEachCmpOfNeighborBDistFromWallSQ(currentBX, currentBY, currentBZ,
														'y', distFromWallSQs[e_010]))
	{
		distFromWallSQs[e_110] = this->bc_radius_effectiveSQ + EPS;
		distFromWallSQs[e_011] = this->bc_radius_effectiveSQ + EPS;
		distFromWallSQs[e_111] = this->bc_radius_effectiveSQ + EPS;
	}
	if (!this->_StoreEachCmpOfNeighborBDistFromWallSQ(currentBX, currentBY, currentBZ,
														'z', distFromWallSQs[e_001]))
	{
		distFromWallSQs[e_101] = this->bc_radius_effectiveSQ + EPS;
		distFromWallSQs[e_011] = this->bc_radius_effectiveSQ + EPS;
		distFromWallSQs[e_111] = this->bc_radius_effectiveSQ + EPS;
	}
	if (distFromWallSQs[e_110] == -1.0)
	{
		distFromWallSQs[e_110] = this->_GetDistFromWallSQ(currentBX + 1, currentBY + 1, currentBZ);
	}
	if (distFromWallSQs[e_101] == -1.0)
	{
		distFromWallSQs[e_101] = this->_GetDistFromWallSQ(currentBX + 1, currentBY, currentBZ + 1);
	}
	if (distFromWallSQs[e_011] == -1.0)
	{
		distFromWallSQs[e_011] = this->_GetDistFromWallSQ(currentBX, currentBY + 1, currentBZ + 1);
	}
	if (distFromWallSQs[e_111] == -1.0)
	{
		distFromWallSQs[e_111] = this->_GetDistFromWallSQ(currentBX + 1, currentBY + 1, currentBZ + 1);
	}
	return	trilinear_interpolation_dist(pPos, currentBX, currentBY, currentBZ, 
									distFromWallSQs[e_000], distFromWallSQs[e_100], distFromWallSQs[e_010], distFromWallSQs[e_110], 
									distFromWallSQs[e_001], distFromWallSQs[e_101], distFromWallSQs[e_011], distFromWallSQs[e_111]);
}

void	BC::MoveVertexToMapCenterBs(const Vec &halfMapSize, const double midHeight)
{
	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		move_vec_to_map_center(this->buckets[i].position, halfMapSize, midHeight);
	}
}

void	BC::DrawDisFromWallSQ(const Vec &halfMapSize, const double midHeight)
{
	const double	maxDis = this->bc_radius_effectiveSQ;
	const double	midDis = maxDis / 2.0;
	
	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		if (this->buckets[i].distFromWallSQ < maxDis)
		{
			glPointSize(5.0f);
			glBegin(GL_POINTS);
			if (this->buckets[i].distFromWallSQ < midDis)
			{
				glColor3f(0, this->buckets[i].distFromWallSQ / midDis, 1 - this->buckets[i].distFromWallSQ / midDis);
			}
			else
			{
				glColor3f(this->buckets[i].distFromWallSQ / midDis, 1 - this->buckets[i].distFromWallSQ / midDis, 0);
			}
			drawVertex(move_vec_to_map_center(this->buckets[i].position, halfMapSize, midHeight));
			glEnd();
		}

		glShadeModel(GL_SMOOTH);
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);
		// drawVertex(move_vec_to_map_center(this->buckets[i].position, halfMapSize, midHeight));
		// drawVertex(move_vec_to_map_center(this->buckets[i].position + this->buckets[i].n * 1000, halfMapSize, midHeight));
		drawVertex(move_vec_to_map_center(this->buckets[i].center, halfMapSize, midHeight));
		glColor3f(0.5f, 0.0f, 0.0f);
		drawVertex(move_vec_to_map_center(this->buckets[i].center + this->buckets[i].nInterpolation * 1000, halfMapSize, midHeight));
		Print::OutWords(this->buckets[i].nInterpolation);
		glEnd();

		// if (this->buckets[i].bucketX == 13 &&
		// 	this->buckets[i].bucketY == 12 &&
		// 	this->buckets[i].bucketZ == 0)
		// {
		// 	if (this->buckets[i].distFromWallSQ < maxDis)
		// 	{
		// 		glPointSize(5.0f);
		// 		glBegin(GL_POINTS);
		// 		if (this->buckets[i].distFromWallSQ < midDis)
		// 		{
		// 			glColor3f(0, this->buckets[i].distFromWallSQ / midDis, 1 - this->buckets[i].distFromWallSQ / midDis);
		// 		}
		// 		else
		// 		{
		// 			glColor3f(this->buckets[i].distFromWallSQ / midDis, 1 - this->buckets[i].distFromWallSQ / midDis, 0);
		// 		}
		// 		drawVertex(move_vec_to_map_center(this->buckets[i].position, halfMapSize, midHeight));
		// 		glEnd();
		// 	}

		// 	glShadeModel(GL_SMOOTH);
		// 	glBegin(GL_LINES);
		// 	glColor3f(1.0f, 1.0f, 1.0f);
		// 	// drawVertex(move_vec_to_map_center(this->buckets[i].position, halfMapSize, midHeight));
		// 	// drawVertex(move_vec_to_map_center(this->buckets[i].position + this->buckets[i].n * 1000, halfMapSize, midHeight));
		// 	drawVertex(move_vec_to_map_center(this->buckets[i].center, halfMapSize, midHeight));
		// 	glColor3f(0.5f, 0.0f, 0.0f);
		// 	drawVertex(move_vec_to_map_center(this->buckets[i].center + this->buckets[i].nInterpolation * 1000, halfMapSize, midHeight));
		// 	Print::OutWords(this->buckets[i].nInterpolation);
		// 	glEnd();
		// }
	}
}

// void	BC::_SearchNeighborParticle(const size_t i)
// {
// 	size_t	currentBX = size_t(ToBucketCoor(this->ps[i].center.x));
// 	size_t	currentBY = size_t(ToBucketCoor(this->ps[i].center.y));
// 	size_t	currentBZ = size_t(ToBucketCoor(this->ps[i].center.z));

// 	size_t	otherBX = _InitOtherBucketCoor(currentBX);
// 	size_t	otherBY = _InitOtherBucketCoor(currentBY);
// 	size_t	otherBZ = _InitOtherBucketCoor(currentBZ);

// 	size_t	maxBX = _InitMaxOtherBucketCoor(this->bucketRow,    currentBX);
// 	size_t	maxBY = _InitMaxOtherBucketCoor(this->bucketColumn, currentBY);
// 	size_t	maxBZ = _InitMaxOtherBucketCoor(this->bucketDepth,  currentBZ);

// 	size_t	bucketIdx;
// 	size_t	particleIdx;

// 	for (; otherBX <= maxBX ; ++otherBX){
// 	for (; otherBY <= maxBY ; ++otherBY){
// 	for (; otherBZ <= maxBZ ; ++otherBZ){
// 		bucketIdx = this->_CalcBucketIdx(otherBX, otherBY, otherBZ);
// 		particleIdx = this->buckets[bucketIdx].firstPrtIdx;
// 		if (particleIdx == UINT64_MAX)
// 		{
// 			continue;
// 		}
// 		for (;;)
// 		{

// 			particleIdx = this->particleNextIdxs[particleIdx];
// 			if (particleIdx == UINT64_MAX)
// 			{
// 				break;
// 			}
// 		}
// 	}}}
// }

// BC::BC(const BC &BC)
// {
// 	*this = BC;
// }

// bool	BC::operator==(const BC &BC) const
// {
// 	return (this->x == BC.x) && 
// 		   (this->y == BC.y) &&
// 		   (this->r == BC.r);
// }

// BC&	BC::operator=(const BC &BC)
// {
// 	if (this != &BC)
// 	{
// 		this->x = BC.x;
// 		this->y = BC.y;
// 		this->r = BC.r;
// 	}
// 	return *this;
// }

// std::ostream &operator<<(std::ostream &ostrm, const BC &BC)
// {
// 	return ostrm << '(' << BC.x << ", " 
// 						<< BC.y << ", "
// 						<< BC.r << ')' 
// 						<< std::endl;
// }
