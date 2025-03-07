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
	   :_visibleMapSize(visibleMapSize_) ,
	    _totalMapSize(totalMapSize_),
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

size_t	BC::_CalcBucketIdx(size_t bucketX, size_t bucketY, size_t bucketZ)
{
	return bucketX + 
		   this->bucketColumn * bucketY + 
		   this->_columnMultiplDepth * bucketZ;
}

size_t	BC::_CalcBucketIdx(const Vec &v)
{
	return this->_CalcBucketIdx(v.x, v.y, v.z);
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
		bucketX = size_t(ps[i].center.x / BUCKET_LENGTH);
		bucketY = size_t(ps[i].center.y / BUCKET_LENGTH);
		bucketZ = size_t(ps[i].center.z / BUCKET_LENGTH);

		bucketIdx = this->_CalcBucketIdx(bucketX, bucketY, bucketZ);
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
	size_t	bucketX;
	size_t	bucketY;
	size_t	bucketZ;
	size_t	bucketXY;

	bucketZ = i / this->_columnMultiplDepth;
	this->buckets[i].position.z = BUCKET_LENGTH * bucketZ;

	bucketXY = i % this->_columnMultiplDepth;	
	bucketY = bucketXY / this->bucketColumn;
	this->buckets[i].position.y = BUCKET_LENGTH * bucketY;
	
	bucketX = bucketXY % this->bucketColumn;
	this->buckets[i].position.x = BUCKET_LENGTH * bucketX;

	this->buckets[i].position.x += BUCKET_LENGTH / 2.0;
	this->buckets[i].position.y += BUCKET_LENGTH / 2.0;
	this->buckets[i].position.z += BUCKET_LENGTH / 2.0;
}

void	BC::_MakeBuckets(const std::deque<Particle> &ps)
{
	this->buckets           = new t_bucket[this->numOfBuckets];
	this->_bucketLast       = new t_bucket[this->numOfBuckets];
	this->particleNextIdxs  = new size_t[NUM_OF_PARTICLES];

	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		this->buckets[i].firstPrtIdx     = UINT64_MAX;
		this->buckets[i].wallWeight      = 0.0;
		this->_bucketLast[i].firstPrtIdx = UINT64_MAX;

		this->_CalcBucketsPos(i);
		
		if (i < NUM_OF_PARTICLES)
		{
			this->particleNextIdxs[i] = UINT64_MAX;
		}
	}
	this->_UpdateParticlesInBuckets(ps);
}

void	BC::_UpdateBuckets(const std::deque<Particle> &ps)
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

double	BC::_CalcDistanceFromTriangle(const Triangle &t, const size_t bucketIdx)
{
	const Vec		apVec =  this->buckets[bucketIdx].position - t.a;
	const double	coefficient = apVec.DotProduct3d(t.n) / t.n.MagnitudeSQ3d();
	const Vec 		orientVec = t.n * coefficient;
	const Vec		projectivePoint = this->buckets[bucketIdx].position - orientVec;
	
	if (t.InternalAndExternalJudgments3d(projectivePoint))
	{
		// std::cout << apVec.DotProduct3d(t.n) << " " << t.n.MagnitudeSQ3d() << std::endl;
		
		return orientVec.MagnitudeSQ3d();
	}
	return 2.0 * BUCKET_LENGTH * 2.0 * BUCKET_LENGTH;
}

double	BC::_CalcShortestDistanceSQ(const Triangle &t, const size_t bucketIdx)
{
	// const double	disFromVertex   = this->_CalcShortestDistanceFromVertexSQ(t, bucketIdx);
	// return disFromVertex;

	// const double	disFromVertex   = this->_CalcShortestDistanceFromVertexSQ(t, bucketIdx);
	// const double	disFromSide 	= this->_CalcShortestDistanceFromSideSQ(t,bucketIdx);
	// return min(disFromVertex, disFromSide);

	const double	disFromVertex   = this->_CalcShortestDistanceFromVertexSQ(t, bucketIdx);
	const double	disFromSide 	= this->_CalcShortestDistanceFromSideSQ(t,bucketIdx);
	const double	disFromTriangle = this->_CalcDistanceFromTriangle(t, bucketIdx);

	return min_of_3_elm(disFromVertex, disFromSide, disFromTriangle);
}

double	BC::_CalcWallWeight(const double disFromWall)
{
	if (disFromWall < 0.0)
	{
		Print::Err("_CalcWallWeight: disFromWall");
	}
	if (this->_weights.back().x < disFromWall + EPS)
	{
		return 0.0;
	}
	Vec nextWeight;
	for (size_t	i = 0; i + 1 < this->_weights.size(); ++i)
	{
		nextWeight = this->_weights[i + 1];
		if (disFromWall <= nextWeight.x)
		{
			return	this->_weights[i].Interpolate2d(nextWeight, disFromWall);
		}
	}
	Print::Err("_CalcWallWeight: weight");
	return 0.0;
}

void	BC::_CalcDistanceFromWall(const Triangle &t)
{
	const Vec	maxCrd = this->_MaxEachCoordinateOfVertex(t.a, t.b, t.c);
	const Vec	minCrd = this->_MinEachCoordinateOfVertex(t.a, t.b, t.c);

	size_t	bucketX = size_t(minCrd.x / BUCKET_LENGTH);
	size_t	bucketY = size_t(minCrd.y / BUCKET_LENGTH);
	size_t	bucketZ = size_t(minCrd.z / BUCKET_LENGTH);
	size_t	bucketIdx;

	double	shortestDistance;
	double	wallWeight;
	
	for (double	i = minCrd.x; size_t(i) <= size_t(maxCrd.x);) {
	for (double	j = minCrd.y; size_t(j) <= size_t(maxCrd.y);) {
	for (double	k = minCrd.z; size_t(k) <= size_t(maxCrd.z);) {
				bucketIdx        = this->_CalcBucketIdx(bucketX, bucketY, bucketZ);
				shortestDistance = sqrt(this->_CalcShortestDistanceSQ(t, bucketIdx));
				wallWeight 	     = this->_CalcWallWeight(shortestDistance);
				if (this->buckets[bucketIdx].wallWeight < wallWeight)
				{
					this->buckets[bucketIdx].wallWeight = wallWeight;
				}
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

void	BC::_CalcAllDistanceFromWall(const std::deque<Triangle>	&ts)
{
	for (size_t	i = 0; i < ts.size(); ++i)
	{
		// std::cout << i << " " << ts[i] << std::endl << std::endl;
		this->_CalcDistanceFromWall(ts[i]);
	}
}

void	BC::MoveVertexToMapCenterBs(const Vec &halfMapSize, const double midHeight)
{
	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		move_vec_to_map_center(this->buckets[i].position, halfMapSize, midHeight);
	}
}

void	BC::RotationBs(void)
{
	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		rotation(this->buckets[i].position);
	}
}

void	BC::DrawDisFromWall(const Vec &halfMapSize, const double midHeight)
{
	glPointSize(2.0f);
	glBegin(GL_POINTS);
	double	maxWeight = this->_weights[0].y;
	double	midWeight = maxWeight / 2.0;
	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		if (0.0 < this->buckets[i].wallWeight)
		{
			if (this->buckets[i].wallWeight < midWeight)
			{
				glColor3f(0, this->buckets[i].wallWeight / midWeight, 1 - this->buckets[i].wallWeight / midWeight);
			}
			else
			{
				glColor3f(this->buckets[i].wallWeight / midWeight, 1 - this->buckets[i].wallWeight / midWeight, 0);
			}
			drawVertex(move_vec_to_map_center(this->buckets[i].position, halfMapSize, midHeight));
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