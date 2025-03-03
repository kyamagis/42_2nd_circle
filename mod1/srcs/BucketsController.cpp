#include "../includes/BucketsController.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Defines.hpp"
#include "../includes/Graphic.hpp"

// BC::BC()
// {
	
// }

BC::BC(const Vec &visibleMapSize_,
	   const Vec &totalMapSize_)
	   :_visibleMapSize(visibleMapSize_) ,
	    _totalMapSize(totalMapSize_),
		bucketRow(to_bucket_coor(this->_totalMapSize.x)),
		bucketColumn(to_bucket_coor(this->_totalMapSize.y)),
		bucketDepth(to_bucket_coor(this->_totalMapSize.z)),
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

void	BC::_MakeBuckets(const std::deque<Particle> &ps)
{
	std::cout << this->numOfBuckets << std::endl;
	this->buckets           = new t_bucket[this->numOfBuckets];
	t_bucket	*bucketLast = new t_bucket[this->numOfBuckets];
	this->particleNextIdxs  = new int64_t[NUM_OF_PARTICLES];

	size_t	bucketX;
	size_t	bucketY;
	size_t	bucketZ;

	size_t	bucketXY;

	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		this->buckets[i].firstPrtIdx   = -1;
		this->buckets[i].disFromWall = 2.0 * BUCKET_LENGTH;
		bucketLast[i].firstPrtIdx   = -1;
		bucketLast[i].disFromWall = 2.0 * BUCKET_LENGTH;

		bucketZ = i / this->_columnMultiplDepth;
		this->buckets[i].position.z = BUCKET_LENGTH * bucketZ;

		bucketXY = i % this->_columnMultiplDepth;	
		bucketY = bucketXY / this->bucketColumn;
		this->buckets[i].position.y = BUCKET_LENGTH * bucketY;
		
		bucketX = bucketXY % this->bucketColumn;
		this->buckets[i].position.x = BUCKET_LENGTH * bucketX;
		
		if (i < NUM_OF_PARTICLES)
		{
			this->particleNextIdxs[i] = -1;
		}
	}

	size_t	bucketIdx;
	int64_t	nextIdx;	

	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		bucketX = size_t(ps[i].center.x / BUCKET_LENGTH);
		bucketY = size_t(ps[i].center.y / BUCKET_LENGTH);
		bucketZ = size_t(ps[i].center.z / BUCKET_LENGTH);

		bucketIdx = this->_CalcBucketIdx(bucketX, bucketY, bucketZ);
		nextIdx = bucketLast[bucketIdx].firstPrtIdx;
		bucketLast[bucketIdx].firstPrtIdx = i;
		if (nextIdx == -1)
		{
			this->buckets[bucketIdx].firstPrtIdx = i;
		}
		else
		{
			this->particleNextIdxs[nextIdx] = i;
		}
		// std::cout << ps[i].center << " " << this->buckets[bucketIdx].position << std::endl;
	}
	delete [] bucketLast;
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

Vec	BC::_CalcBucketCenterPos(const size_t i, const size_t j, const size_t k)
{
	return Vec(i + BUCKET_LENGTH / 2.0, 
			   j + BUCKET_LENGTH / 2.0,
			   k + BUCKET_LENGTH / 2.0);
}

double	BC::_CalcShortestDistanceFromVertex(const Triangle &t, 
									const Vec &bucketCenterPos)
{
	double	disFromVertexA = t.a.Magnitude3d(bucketCenterPos);
	double	disFromVertexB = t.b.Magnitude3d(bucketCenterPos);
	double	disFromVertexC = t.c.Magnitude3d(bucketCenterPos);

	return max_of_3_elm(disFromVertexA, disFromVertexB, disFromVertexC);
}

double	BC::_CalcDistanceFromSide(const Vec &a, 
								  const Vec &b, 
								  const Vec &bucketCenterPos)
{
	const Vec	abVector = b - a;
	const Vec	aCenterVector = bucketCenterPos - a;

	const double	t = abVector.DotProduct3d(aCenterVector) / 
				abVector.MagnitudeSQ3d();

	if (0.0 < t && t < 1.0)
	{
		const Vec	projectivePoint = a + (abVector * t);

		return (bucketCenterPos - projectivePoint).Magnitude3d();
	}
		
	return 2.0 * BUCKET_LENGTH;
}

double	BC::_CalcShortestDistanceFromSide(const Triangle &t,  
								          const Vec &bucketCenterPos)
{
	double	disFromSideAB = this->_CalcDistanceFromSide(t.a, t.b, bucketCenterPos);
	double	disFromSideBC = this->_CalcDistanceFromSide(t.b, t.c, bucketCenterPos);
	double	disFromSideCA = this->_CalcDistanceFromSide(t.c, t.a, bucketCenterPos);

	return max_of_3_elm(disFromSideAB, disFromSideBC, disFromSideCA);
}

double	BC::_CalcDistanceFromTriangle(const Triangle &t,  
									  const Vec &bucketCenterPos)
{
	const Vec		apVec =  bucketCenterPos - t.a;
	const double	disFromTriangle = apVec.DotProduct3d(t.n) / 
									  t.n.MagnitudeSQ3d();
	const Vec		projectivePoint = bucketCenterPos -
								  t.n * disFromTriangle;
								  
	if (t.InternalAndExternalJudgments3d(projectivePoint))
	{
		// std::cout << apVec.DotProduct3d(t.n) << " " << t.n.MagnitudeSQ3d() << std::endl;
		return disFromTriangle;
	}
	return 2.0 * BUCKET_LENGTH;
}

double	BC::_CalcShortestDistance(const Triangle &t,
								  const double i, 
								  const double j, 
							      const double k)
{
	Vec		bucketCenterPos = this->_CalcBucketCenterPos(i,j,k);

	double	disFromVertex   = this->_CalcShortestDistanceFromVertex(t, bucketCenterPos);
	double	disFromSide 	= this->_CalcShortestDistanceFromSide(t,bucketCenterPos);
	double	disFromTriangle = this->_CalcDistanceFromTriangle(t, bucketCenterPos);

	// return disFromVertex;

	return max_of_3_elm(disFromVertex, disFromSide, disFromTriangle);
}

void	BC::_CalcDistanceFromWall(const Triangle &t)
{
	Vec	maxCrd = this->_MaxEachCoordinateOfVertex(t.a, t.b, t.c);
	Vec minCrd = this->_MinEachCoordinateOfVertex(t.a, t.b, t.c);

	size_t	bucketX = size_t(minCrd.x / BUCKET_LENGTH);
	size_t	bucketY = size_t(minCrd.y / BUCKET_LENGTH);
	size_t	bucketZ = size_t(minCrd.z / BUCKET_LENGTH);
	size_t	bucketIdx;

	double	shortestDistance;
	
	for (double	i = minCrd.x; size_t(i) <= size_t(maxCrd.x);) {
	for (double	j = minCrd.y; size_t(j) <= size_t(maxCrd.y);) {
	for (double	k = minCrd.z; size_t(k) <= size_t(maxCrd.z);) {
				bucketIdx = this->_CalcBucketIdx(bucketX, bucketY, bucketZ);
				shortestDistance = this->_CalcShortestDistance(t, i, j, k);
				if (shortestDistance < this->buckets[bucketIdx].disFromWall)
				{
					this->buckets[bucketIdx].disFromWall = shortestDistance;
				}
				k += BUCKET_LENGTH;
				bucketZ = k / BUCKET_LENGTH;
			}
			j += BUCKET_LENGTH;
			bucketY = j / BUCKET_LENGTH;
		}
		i += BUCKET_LENGTH;
		bucketX = i / BUCKET_LENGTH;
		// std::cout << i << std::endl;
	}
}

void	BC::_CalcAllDistanceFromWall(const std::deque<Triangle>	&ts)
{
	for (size_t	i = 0; i < ts.size(); ++i)
	{
		std::cout << i << " " << ts[i] << std::endl;
		this->_CalcDistanceFromWall(ts[i]);
	}
}

size_t	_InitOtherBucketCoor(const size_t coor)
{
	if (0 < coor)
	{
		return  coor - 1;
	}
	return coor;
}

size_t	_InitMaxOtherBucketCoor(const size_t max, const size_t coor)
{
	if (coor < max)
	{
		return  coor + 1;
	}
	return coor;
}


void	BC::DrawDisFromWall(void)
{
	glPointSize(1.0f);
	glBegin(GL_POINTS);
	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		// if (this->buckets[i].disFromWall < 2 *  BUCKET_LENGTH)
		// {
		// 	std::cout << this->buckets[i].disFromWall << std::endl;
		// 	glColor3f(1.0, 1.0, 1.0);
		// 	drawVertex(this->buckets[i].position);
		// }
		// std::cout << this->buckets[i].disFromWall << std::endl;
		glColor3f(0, 1.0 - double(i / double(this->numOfBuckets)), double(i / double(this->numOfBuckets)));
		drawVertex(this->buckets[i].position);
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
// 		if (particleIdx == -1)
// 		{
// 			continue;
// 		}
// 		for (;;)
// 		{

// 			particleIdx = this->particleNextIdxs[particleIdx];
// 			if (particleIdx == -1)
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