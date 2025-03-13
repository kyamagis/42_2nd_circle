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
		bucketX = size_t(ps[i].center.x / BUCKET_LENGTH);
		bucketY = size_t(ps[i].center.y / BUCKET_LENGTH);
		bucketZ = size_t(ps[i].center.z / BUCKET_LENGTH);

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
	this->buckets[i].position.z = BUCKET_LENGTH * this->buckets[i].bucketZ;

	bucketXY = i % this->_columnMultiplDepth;	
	this->buckets[i].bucketY = bucketXY / this->bucketColumn;
	this->buckets[i].position.y = BUCKET_LENGTH * this->buckets[i].bucketY;
	
	this->buckets[i].bucketX = bucketXY % this->bucketColumn;
	this->buckets[i].position.x = BUCKET_LENGTH * this->buckets[i].bucketX;

	this->buckets[i].center += this->buckets[i].position + BUCKET_LENGTH / 2.0;
}



Vec	BC::_MaxEachCoordinateOfVertex(const Vec &a, 
									const Vec &b,
									const Vec &c)
{
	Vec maxCoordinate;

	maxCoordinate.x = max_of_3_elm(a.x, b.x, c.x);
	maxCoordinate.y = max_of_3_elm(a.y, b.y, c.y);
	maxCoordinate.z = max_of_3_elm(a.z, b.z, c.z);

	if (maxCoordinate.x + E_RADIUS < this->_totalMapSize.x)
	{
		maxCoordinate.x += E_RADIUS;
	}
	if (maxCoordinate.y + E_RADIUS < this->_totalMapSize.y)
	{
		maxCoordinate.y += E_RADIUS;
	}
	if (maxCoordinate.z + E_RADIUS< this->_totalMapSize.z)
	{
		maxCoordinate.z += E_RADIUS;
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

	if (E_RADIUS < minCoordinate.x)
	{
		minCoordinate.x -= E_RADIUS;
	}
	if (E_RADIUS < minCoordinate.y)
	{
		minCoordinate.y -= E_RADIUS;
	}
	if (E_RADIUS < minCoordinate.z)
	{
		minCoordinate.z -= E_RADIUS;
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
		
	return 2.0 * BUCKET_LENGTH * 2.0 * BUCKET_LENGTH;
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
	return 2.0 * BUCKET_LENGTH * 2.0 * BUCKET_LENGTH;
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

	size_t	bucketX = size_t(minCrd.x / BUCKET_LENGTH);
	size_t	bucketY = size_t(minCrd.y / BUCKET_LENGTH);
	size_t	bucketZ = size_t(minCrd.z / BUCKET_LENGTH);
	size_t	bucketIdx;

	double	shortestDistSQ;
	
	for (double	i = minCrd.x; size_t(i) <= size_t(maxCrd.x);) {
	for (double	j = minCrd.y; size_t(j) <= size_t(maxCrd.y);) {
	for (double	k = minCrd.z; size_t(k) <= size_t(maxCrd.z);) {
				bucketIdx      = this->BC_CalcBucketIdx(bucketX, bucketY, bucketZ);
				shortestDistSQ = this->_CalcShortestDistanceSQ(t, bucketIdx);
				if (shortestDistSQ - EPS < this->buckets[bucketIdx].distFromWallSQ &&
					this->buckets[bucketIdx].distFromWallSQ < shortestDistSQ + EPS)
				{
					this->buckets[bucketIdx].n += t.n;
					if (shortestDistSQ < this->buckets[bucketIdx].distFromWallSQ)
					{
						this->buckets[bucketIdx].distFromWallSQ = shortestDistSQ;
					}
				}
				else if (shortestDistSQ <= this->buckets[bucketIdx].distFromWallSQ)
				{
					this->buckets[bucketIdx].n = t.n;
					this->buckets[bucketIdx].distFromWallSQ = shortestDistSQ;
				}
				// if (this->buckets[bucketIdx].bucketX == 7 &&
				// 	this->buckets[bucketIdx].bucketY == 7)
				// {
				// 	Print::OutWords();
				// }
				k += BUCKET_LENGTH;
				bucketZ = k / BUCKET_LENGTH;
			}
			bucketZ = size_t(minCrd.z / BUCKET_LENGTH);
			j += BUCKET_LENGTH;
			bucketY = j / BUCKET_LENGTH;
		}
		bucketY = size_t(minCrd.y / BUCKET_LENGTH);
		i += BUCKET_LENGTH;
		bucketX = i / BUCKET_LENGTH;
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
		neighborBDistFromWallSQ = E_RADIUS_SQ + EPS;
	}

	return currentBcmp < maxRCD;
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
		this->buckets[i].distFromWallSQ  = E_RADIUS_SQ + EPS;
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
	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		nVecMagnitude = this->buckets[i].n.Magnitude3d();
		if (EPS < nVecMagnitude)
		{
			this->buckets[i].n /= this->buckets[i].n.Magnitude3d();
		}
		// if (this->buckets[i].bucketX == 7 &&
		// 	this->buckets[i].bucketZ == 0)
		// {
		// 	Print::OutWords("i: ", i, 
		// 					this->buckets[i].bucketX, this->buckets[i].bucketY, this->buckets[i].bucketZ,
		// 					sqrt(this->buckets[i].distFromWallSQ),
		// 					this->buckets[i].n);
		// }
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
			return	this->_weights[i].Interpolate2d(nextWeight, interpolatedDist);
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
		distFromWallSQ_110 = E_RADIUS_SQ + EPS;
		distFromWallSQ_101 = E_RADIUS_SQ + EPS;
		distFromWallSQ_111 = E_RADIUS_SQ + EPS;
	}

	if (!this->_StoreEachCmpOfNeighborBDistFromWallSQ(currentBX, currentBY, currentBZ,
														'y', distFromWallSQ_010))
	{
		distFromWallSQ_110 = E_RADIUS_SQ + EPS;
		distFromWallSQ_011 = E_RADIUS_SQ + EPS;
		distFromWallSQ_111 = E_RADIUS_SQ + EPS;
	}
	if (!this->_StoreEachCmpOfNeighborBDistFromWallSQ(currentBX, currentBY, currentBZ,
														'z', distFromWallSQ_001))
	{
		distFromWallSQ_101 = E_RADIUS_SQ + EPS;
		distFromWallSQ_011 = E_RADIUS_SQ + EPS;
		distFromWallSQ_111 = E_RADIUS_SQ + EPS;
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
	return	trilinear_interpolation(pPos, currentBX, currentBY, currentBZ, 
									distFromWallSQ_000, distFromWallSQ_100, distFromWallSQ_010, distFromWallSQ_110, 
									distFromWallSQ_001, distFromWallSQ_101, distFromWallSQ_011, distFromWallSQ_111);
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
	const double	maxDis = E_RADIUS_SQ;
	const double	midDis = maxDis / 2.0;

	glPointSize(2.0f);
	glBegin(GL_POINTS);
	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		if (this->buckets[i].bucketX == 7 &&
			this->buckets[i].bucketZ == 0)
		{
			if (this->buckets[i].distFromWallSQ < maxDis)
			{
				if (this->buckets[i].distFromWallSQ < midDis)
				{
					glColor3f(0, this->buckets[i].distFromWallSQ / midDis, 1 - this->buckets[i].distFromWallSQ / midDis);
				}
				else
				{
					glColor3f(this->buckets[i].distFromWallSQ / midDis, 1 - this->buckets[i].distFromWallSQ / midDis, 0);
				}
				drawVertex(move_vec_to_map_center(this->buckets[i].position, halfMapSize, midHeight));
			}
		}
	}
	glEnd();
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

// void	BC::_SearchNeighborParticles(const size_t oneself)
// {
// 	// size_t	bucketX = this->ps[oneself].center.x / BUCKET_LENGTH;
// 	// size_t	bucketY = this->ps[oneself].center.y / BUCKET_LENGTH;
// 	// size_t	bucketZ = this->ps[oneself].center.z / BUCKET_LENGTH;

// 	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
// 	{
// 		this->_SearchNeighborParticle(i);

// 	}
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
