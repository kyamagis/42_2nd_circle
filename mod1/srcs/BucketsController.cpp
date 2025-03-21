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

	if (maxCoordinate.x + this->bc_bucketLength < this->_totalMapSize.x)
	{
		maxCoordinate.x += this->bc_bucketLength;
	}
	if (maxCoordinate.y + this->bc_bucketLength < this->_totalMapSize.y)
	{
		maxCoordinate.y += this->bc_bucketLength;
	}
	if (maxCoordinate.z + this->bc_bucketLength < this->_totalMapSize.z)
	{
		maxCoordinate.z += this->bc_bucketLength;
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

	if (this->bc_bucketLength < minCoordinate.x)
	{
		minCoordinate.x -= this->bc_bucketLength;
	}
	if (this->bc_bucketLength < minCoordinate.y)
	{
		minCoordinate.y -= this->bc_bucketLength;
	}
	if (this->bc_bucketLength < minCoordinate.z)
	{
		minCoordinate.z -= this->bc_bucketLength;
	}

	return minCoordinate;
}

Vec	BC::_CalcShortestVecFromVertex(const Triangle &t, const size_t bucketIdx)
{
	return min_of_3_vec(t.a - this->buckets[bucketIdx].position,
						t.b - this->buckets[bucketIdx].position, 
						t.c - this->buckets[bucketIdx].position);
}

Vec	BC::_CalcVecFromSide(const Vec &a, const Vec &b, const size_t bucketIdx)
{
	const Vec	abVector = b - a;
	const Vec	aCenterVector = this->buckets[bucketIdx].position - a;

	const double	t = abVector.DotProduct3d(aCenterVector) / 
						abVector.MagnitudeSQ3d();

	if (0.0 < t && t < 1.0)
	{
		const Vec	projectivePoint = a + (abVector * t);

		return (projectivePoint - this->buckets[bucketIdx].position);
	}

	return Vec(1,1,1) * (this->bc_bucketLength + EPS);
}

Vec	BC::_CalcShortestVecFromSide(const Triangle &t, const size_t bucketIdx)
{
	const Vec	vecFromSideAB = this->_CalcVecFromSide(t.a, t.b, bucketIdx);
	const Vec	vecFromSideBC = this->_CalcVecFromSide(t.b, t.c, bucketIdx);
	const Vec	vecFromSideCA = this->_CalcVecFromSide(t.c, t.a, bucketIdx);

	return min_of_3_vec(vecFromSideAB, vecFromSideBC, vecFromSideCA);
}

Vec	BC::_CalcVecFromTriangle(const Triangle &t, const size_t bucketIdx)
{
	const Vec		apVec = this->buckets[bucketIdx].position - t.a;
	const double	coefficient = apVec.DotProduct3d(t.n) / t.n.MagnitudeSQ3d();
	const Vec 		orientVec =  t.n * -1 * coefficient;
	const Vec		projectivePoint = this->buckets[bucketIdx].position + orientVec;

	if (t.InternalAndExternalJudgments3d(projectivePoint))
	{
		return orientVec;
	}
	return Vec(1,1,1) * (this->bc_bucketLength + 1);
}

Vec	BC::_CalcShortestVec(const Triangle &t, const size_t bucketIdx)
{
	// const double	distFromVertex   = this->_CalcShortestDistanceFromVertexSQ(t, bucketIdx);
	// return distFromVertex;

	// const double	distFromVertex   = this->_CalcShortestDistanceFromVertexSQ(t, bucketIdx);
	// const double	distFromSide 	= this->_CalcShortestDistanceFromSideSQ(t,bucketIdx);
	// return min(distFromVertex, distFromSide);

	const Vec	vecFromVertex   = this->_CalcShortestVecFromVertex(t, bucketIdx);
	const Vec	vecFromSide 	= this->_CalcShortestVecFromSide(t,bucketIdx);
	const Vec	vecFromTriangle = this->_CalcVecFromTriangle(t, bucketIdx);
	const Vec	shortestVec   = min_of_3_vec(vecFromVertex, vecFromSide, vecFromTriangle);

	return shortestVec;
}

double	BC::_GetDistFromWall(const size_t currentBX,
						 	   const size_t currentBY,
						 	   const size_t currentBZ)
{
	const size_t	neighborBIdx = this->BC_CalcBucketIdx(currentBX, currentBY, currentBZ);
	return this->buckets[neighborBIdx].distFromWall;
}

double	BC::_SearchNeighborBDistFromWall(size_t currentBX,
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
	return this->_GetDistFromWall(currentBX, currentBY, currentBZ);
}

bool	BC::_StoreEachCmpOfNeighborBDistFromWall(const size_t currentBX,
											const size_t currentBY,
											const size_t currentBZ,
											const unsigned char cmp,
											double &neighborBDistFromWall)
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
		neighborBDistFromWall = 
		this->_SearchNeighborBDistFromWall(currentBX, currentBY, currentBZ,
											  cmp);
	}
	else
	{
		neighborBDistFromWall = -(this->bc_bucketLength + EPS);
	}

	return currentBcmp + 1 < maxRCD;
}

void	BC::_GetNeighborBDistFromWalls(const size_t bucketIdx, double *distFromWalls)
{
	const double	currentBX = this->buckets[bucketIdx].bucketX;
	const double	currentBY = this->buckets[bucketIdx].bucketY;
	const double	currentBZ = this->buckets[bucketIdx].bucketZ;

	distFromWalls[e_000] = this->buckets[bucketIdx].distFromWall;
	distFromWalls[e_110] = -1.0;
	distFromWalls[e_101] = -1.0;
	distFromWalls[e_111] = -1.0;
	distFromWalls[e_011] = -1.0;

	if (!this->_StoreEachCmpOfNeighborBDistFromWall(currentBX, currentBY, currentBZ,
														'x', distFromWalls[e_100]))
	{
		distFromWalls[e_110] = -(this->bc_bucketLength + EPS);
		distFromWalls[e_101] = -(this->bc_bucketLength + EPS);
		distFromWalls[e_111] = -(this->bc_bucketLength + EPS);
	}

	if (!this->_StoreEachCmpOfNeighborBDistFromWall(currentBX, currentBY, currentBZ,
														'y', distFromWalls[e_010]))
	{
		distFromWalls[e_110] = -(this->bc_bucketLength + EPS);
		distFromWalls[e_011] = -(this->bc_bucketLength + EPS);
		distFromWalls[e_111] = -(this->bc_bucketLength + EPS);
	}
	if (!this->_StoreEachCmpOfNeighborBDistFromWall(currentBX, currentBY, currentBZ,
														'z', distFromWalls[e_001]))
	{
		distFromWalls[e_101] = -(this->bc_bucketLength + EPS);
		distFromWalls[e_011] = -(this->bc_bucketLength + EPS);
		distFromWalls[e_111] = -(this->bc_bucketLength + EPS);
	}
	if (distFromWalls[e_110] == -1.0)
	{
		distFromWalls[e_110] = this->_GetDistFromWall(currentBX + 1, currentBY + 1, currentBZ);
	}
	if (distFromWalls[e_101] == -1.0)
	{
		distFromWalls[e_101] = this->_GetDistFromWall(currentBX + 1, currentBY, currentBZ + 1);
	}
	if (distFromWalls[e_011] == -1.0)
	{
		distFromWalls[e_011] = this->_GetDistFromWall(currentBX, currentBY + 1, currentBZ + 1);
	}
	if (distFromWalls[e_111] == -1.0)
	{
		distFromWalls[e_111] = this->_GetDistFromWall(currentBX + 1, currentBY + 1, currentBZ + 1);
	}
}

void	BC::_CalcDistanceFromWallSQ(const Triangle &t)
{
	const Vec	maxCrd = this->_MaxEachCoordinateOfVertex(t.a, t.b, t.c);
	const Vec	minCrd = this->_MinEachCoordinateOfVertex(t.a, t.b, t.c);

	size_t	bucketX = size_t(minCrd.x / this->bc_bucketLength);
	size_t	bucketY = size_t(minCrd.y / this->bc_bucketLength);
	size_t	bucketZ = size_t(minCrd.z / this->bc_bucketLength);
	size_t	bucketIdx;

	double	shortestDist;
	double	shortestDistAbs;
	double	distFromWallAbs;
	Vec		shortestVec;

	for (double	i = minCrd.x; size_t(i) <= size_t(maxCrd.x);) {
	for (double	j = minCrd.y; size_t(j) <= size_t(maxCrd.y);) {
	for (double	k = minCrd.z; size_t(k) <= size_t(maxCrd.z);) {
				bucketIdx    = this->BC_CalcBucketIdx(bucketX, bucketY, bucketZ);
				shortestVec  = this->_CalcShortestVec(t, bucketIdx);
				shortestDist = shortestVec.Magnitude3d();
				shortestDistAbs = shortestDist;
				distFromWallAbs = abs(this->buckets[bucketIdx].distFromWall);
				if (0.0 < t.n.DotProduct3d(shortestVec.Normalized3d()))
				{
					shortestDist = -shortestDist;
				}
				if (abs(shortestDistAbs - distFromWallAbs) < EPS)
				{
					this->buckets[bucketIdx].n += t.n;
					this->buckets[bucketIdx].shortestVec  = shortestVec;
					this->buckets[bucketIdx].distFromWall = shortestDist;
				}
				else if (shortestDistAbs < distFromWallAbs + EPS)
				{
					this->buckets[bucketIdx].n = t.n;
					this->buckets[bucketIdx].shortestVec  = shortestVec;
					this->buckets[bucketIdx].distFromWall = shortestDist;
				}
				k += this->bc_bucketLength;
				bucketZ = size_t(k / this->bc_bucketLength);
			}
			bucketZ = size_t(minCrd.z / this->bc_bucketLength);
			j += this->bc_bucketLength;
			bucketY = size_t(j / this->bc_bucketLength);
		}
		bucketY = size_t(minCrd.y / this->bc_bucketLength);
		i += this->bc_bucketLength;
		bucketX = size_t(i / this->bc_bucketLength);
	}
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
		this->buckets[i].distFromWall    = this->bc_bucketLength;
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
	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		this->buckets[i].n = this->buckets[i].n.Normalized3d();
		this->_GetNeighborBDistFromWalls(i, this->buckets[i].distFromWalls);
		this->buckets[i].nInterpolation = calc_n_vec(this->buckets[i].distFromWalls);
		// if (this->buckets[i].nInterpolation.z < 0.0)
		// {
		// 	this->buckets[i].nInterpolation *= -1.0;
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

	if (interpolatedDist <= 0.0)
	{
		return this->_weights.front().y;
	}
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

void	DrawShortestVec(const Vec &halfMapSize, const double midHeight, const t_bucket bucket)
{
	glShadeModel(GL_SMOOTH);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawVertex(move_vec_to_map_center(bucket.position, halfMapSize, midHeight));
	glColor3f(0.0f, 1.0f, 0.0f);
	drawVertex(move_vec_to_map_center(bucket.position + bucket.shortestVec, halfMapSize, midHeight));
	glEnd();
}

void	DrawNormalVec(const Vec &halfMapSize, const double midHeight, const t_bucket bucket)
{
	glShadeModel(GL_SMOOTH);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawVertex(move_vec_to_map_center(bucket.position, halfMapSize, midHeight));
	glColor3f(0.0f, 0.0f, 1.0f);
	drawVertex(move_vec_to_map_center(bucket.position + bucket.n * 1000, halfMapSize, midHeight));
	glEnd();
}

void	DrawInterpolatedNormalVec(const Vec &halfMapSize, const double midHeight, const t_bucket bucket)
{
	glShadeModel(GL_SMOOTH);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawVertex(move_vec_to_map_center(bucket.position, halfMapSize, midHeight));
	glColor3f(1.0f, 0.0f, 0.0f);
	drawVertex(move_vec_to_map_center(bucket.position + bucket.nInterpolation * 1000, halfMapSize, midHeight));
	glEnd();
}

void	DrawBucketPos(const Vec &halfMapSize, const double midHeight, const t_bucket bucket)
{
	const double	midDist = BUCKET_LENGTH / 2.0;

	glPointSize(5.0f);
	glBegin(GL_POINTS);
	if (abs(bucket.distFromWall) < midDist)
	{
		glColor3f(0, abs(bucket.distFromWall) / midDist, 1 - abs(bucket.distFromWall) / midDist);
	}
	else
	{
		glColor3f(abs(bucket.distFromWall) / midDist, 1 - abs(bucket.distFromWall) / midDist, 0);
	}
	drawVertex(move_vec_to_map_center(bucket.position, halfMapSize, midHeight));
	glEnd();
}

#include <unordered_map>
std::unordered_map<size_t, std::string> coorToString = {
    {0, "e_000"},
    {1, "e_100"},
    {2, "e_110"},
    {3, "e_101"},
    {4, "e_111"},
    {5, "e_010"},
    {6, "e_011"},
    {7, "e_001"}
};

void	BC::DrawDisFromWallSQ(const Vec &halfMapSize, const double midHeight)
{	
	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		// if (1 <= this->buckets[i].bucketX && this->buckets[i].bucketX + 3 < this->bucketRow &&
		// 	1 <= this->buckets[i].bucketY && this->buckets[i].bucketY + 3 < this->bucketColumn &&
		// 	this->buckets[i].bucketZ <= 4)
		// if (2 <= this->buckets[i].bucketX && this->buckets[i].bucketX + 3 < this->bucketRow &&
		// 	2 <= this->buckets[i].bucketY && this->buckets[i].bucketY + 3 < this->bucketColumn &&
		// 	this->buckets[i].bucketZ < this->bucketDepth)
		if (8 <= this->buckets[i].bucketX && this->buckets[i].bucketX <= 9 &&
			8 <= this->buckets[i].bucketY && this->buckets[i].bucketY <= 8 &&
			1 <= this->buckets[i].bucketZ && this->buckets[i].bucketZ <= 2)
		{	
			if (abs(this->buckets[i].distFromWall) <= this->bc_bucketLength)
			{
				DrawBucketPos(halfMapSize, midHeight, this->buckets[i]);
				DrawShortestVec(halfMapSize, midHeight, this->buckets[i]);
				// DrawNormalVec(halfMapSize, midHeight, this->buckets[i]);
				// DrawInterpolatedNormalVec(halfMapSize, midHeight, this->buckets[i]);
			}
		}
	}
	size_t	bi = this->BC_CalcBucketIdx(8, 7, 1);
	for (size_t	i = 0; i < 8; ++i)
	{
		Print::OutWords(bi, coorToString[i], this->buckets[bi].distFromWalls[i], 
						this->buckets[bi].shortestVec, 
						this->buckets[bi].nInterpolation);
	}
	Print::OutWords("/////////////");
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
/*
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

	
		return (projectivePoint - this->buckets[bucketIdx].position).MagnitudeSQ3d();
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
	const Vec		apVec = this->buckets[bucketIdx].position - t.a;
	const double	coefficient = apVec.DotProduct3d(t.n) / t.n.MagnitudeSQ3d();
	const Vec 		orientVec =  t.n * -1 * coefficient;
	const Vec		projectivePoint = this->buckets[bucketIdx].position + orientVec;
	
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
}*/