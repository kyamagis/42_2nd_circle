#include "../includes/ParticleMethod.hpp"
#include "../includes/MathUtils.hpp"

#define NUM_OF_PARTICLES 100
#define	D 3 // dimension number
#define	GRADIENT  0
#define LAPLACIAN 1
#define	BUCKET_LENGTH  E_RADIUS
#define	NUM_OF_BUCKETS
#define InvisibleSpace 2.0 * BUCKET_LENGTH

PM::PM()
{
	this->ps.resize(NUM_OF_PARTICLES);
}

PM::PM(const uint32_t	mapSize[3], 
	   const int64_t	maxHeight,
	   const std::deque<Triangle>	&ts):visibleMapSize(mapSize[X], mapSize[Y], mapSize[Z]) ,
								bucketFirst(NULL),
								particleNextIdxs(NULL)
{
	(void)maxHeight;
	this->totalMapSize.x = mapSize[X] + 2.0 * InvisibleSpace;
	this->totalMapSize.y = mapSize[Y] + 2.0 * InvisibleSpace;
	this->totalMapSize.z = mapSize[Z] + 2.0 * InvisibleSpace;
	this->ps.resize(NUM_OF_PARTICLES);

	this->bucketRow    = this->totalMapSize.x / BUCKET_LENGTH;
	this->bucketColumn = this->totalMapSize.y / BUCKET_LENGTH;
	this->bucketDepth  = this->totalMapSize.z / BUCKET_LENGTH;
	this->numOfBuckets = this->bucketRow * this->bucketColumn * this->bucketDepth;

	this->_InitBuckets();
	this->_CalcAllDistanceFromWall(ts);
}

PM::~PM()
{
	if (this->bucketFirst != NULL)
	{
		delete this->bucketFirst;
	}
	if (this->particleNextIdxs != NULL)
	{
		delete this->particleNextIdxs;
	}
}

size_t	PM::_CalcBucketIdx(size_t bucketX, size_t bucketY, size_t bucketZ)
{
	return bucketX + 
			this->bucketColumn * bucketY + 
			this->bucketColumn * this->bucketDepth * bucketZ;
}

size_t	PM::_CalcBucketIdx(const Vec &v)
{
	return this->_CalcBucketIdx(v.x, v.y, v.z);
}

void	PM::_InitBuckets(void)
{
	this->bucketFirst       = new t_bucket[this->numOfBuckets];
	t_bucket	*bucketLast = new t_bucket[this->numOfBuckets];
	this->particleNextIdxs  = new int64_t[NUM_OF_PARTICLES];

	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		this->bucketFirst[i].firstPrtIdx   = -1;
		this->bucketFirst[i].disFromWall = 2.0 * BUCKET_LENGTH;
		bucketLast[i].firstPrtIdx   = -1;
		bucketLast[i].disFromWall = 2.0 * BUCKET_LENGTH;
		if (i < NUM_OF_PARTICLES)
		{
			this->particleNextIdxs[i] = -1;
		}
	}

	size_t	bucketX;
	size_t	bucketY;
	size_t	bucketZ;
	size_t	bucketIdx;
	int64_t	nextIdx;	

	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		bucketX = this->ps[i].center.x / BUCKET_LENGTH;
		bucketY = this->ps[i].center.y / BUCKET_LENGTH;
		bucketZ = this->ps[i].center.z / BUCKET_LENGTH;
	
		bucketIdx = bucketX + 
					this->bucketColumn * bucketY + 
					this->bucketColumn * this->bucketDepth * bucketZ;
		nextIdx = bucketLast[bucketIdx].firstPrtIdx;
		bucketLast[bucketIdx].firstPrtIdx = i;
		if (nextIdx == -1)
		{
			this->bucketFirst[bucketIdx].firstPrtIdx = i;
		}
		else
		{
			this->particleNextIdxs[nextIdx] = i;
		}
	}
}

Vec	PM::_MaxEachCoordinateOfVertex(const Vec &a, 
									const Vec &b,
									const Vec &c)
{
	Vec maxCoordinate;

	maxCoordinate.x = max_of_3_elm(a.x, b.x, c.x);
	maxCoordinate.y = max_of_3_elm(a.y, b.y, c.y);
	maxCoordinate.z = max_of_3_elm(a.z, b.z, c.z);

	if (maxCoordinate.x + E_RADIUS < this->totalMapSize.x)
	{
		maxCoordinate.x += E_RADIUS;
	}
	if (maxCoordinate.y + E_RADIUS < this->totalMapSize.y)
	{
		maxCoordinate.y += E_RADIUS;
	}
	if (maxCoordinate.z + E_RADIUS< this->totalMapSize.z)
	{
		maxCoordinate.z += E_RADIUS;
	}

	return maxCoordinate;
}

Vec	PM::_MinEachCoordinateOfVertex(const Vec &a, 
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

Vec	PM::_CalcBucketCenterPos(const size_t i, const size_t j, const size_t k)
{
	return Vec(i + BUCKET_LENGTH / 2.0, 
			   j + BUCKET_LENGTH / 2.0,
			   k + BUCKET_LENGTH / 2.0);
}

double	PM::_CalcShortestDistanceFromVertex(const Triangle &t, 
									const Vec &bucketCenterPos)
{
	double	disFromVertexA = t.a.Magnitude3d(bucketCenterPos);
	double	disFromVertexB = t.b.Magnitude3d(bucketCenterPos);
	double	disFromVertexC = t.c.Magnitude3d(bucketCenterPos);

	return max_of_3_elm(disFromVertexA, disFromVertexB, disFromVertexC);
}

double	PM::_CalcDistanceFromSide(const Vec &a, 
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

double	PM::_CalcShortestDistanceFromSide(const Triangle &t,  
								          const Vec &bucketCenterPos)
{
	double	disFromSideAB = this->_CalcDistanceFromSide(t.a, t.b, bucketCenterPos);
	double	disFromSideBC = this->_CalcDistanceFromSide(t.b, t.c, bucketCenterPos);
	double	disFromSideCA = this->_CalcDistanceFromSide(t.c, t.a, bucketCenterPos);

	return max_of_3_elm(disFromSideAB, disFromSideBC, disFromSideCA);
}

double	PM::_CalcDistanceFromTriangle(const Triangle &t,  
									  const Vec &bucketCenterPos)
{
	const Vec	apVec =  bucketCenterPos - t.a;
	const double	disFromTriangle = apVec.DotProduct3d(t.n) / 
									  t.n.MagnitudeSQ3d();
	const Vec	projectivePoint = bucketCenterPos -
								  t.n * disFromTriangle;

	if (t.InternalAndExternalJudgments3d(projectivePoint))
	{
		return disFromTriangle;
	}
	return 2.0 * BUCKET_LENGTH;
}

double	PM::_CalcShortestDistance(const Triangle &t,
								  const double i, 
								  const double j, 
							      const double k)
{
	Vec		bucketCenterPos = this->_CalcBucketCenterPos(i,j,k);

	double	disFromVertex   = this->_CalcShortestDistanceFromVertex(t, bucketCenterPos);
	double	disFromSide 	= this->_CalcShortestDistanceFromSide(t,bucketCenterPos);
	double	disFromTriangle = this->_CalcDistanceFromTriangle(t, bucketCenterPos);

	return max_of_3_elm(disFromVertex, disFromSide, disFromTriangle);
}

void	PM::_CalcDistanceFromWall(const Triangle &t)
{
	Vec	maxCrd = this->_MaxEachCoordinateOfVertex(t.a, t.b, t.c);
	Vec minCrd = this->_MinEachCoordinateOfVertex(t.a, t.b, t.c);

	size_t	bucketX = size_t(minCrd.x / BUCKET_LENGTH);
	size_t	bucketY = size_t(minCrd.y / BUCKET_LENGTH);
	size_t	bucketZ = size_t(minCrd.z / BUCKET_LENGTH);
	size_t	bucketIdx;

	double	shortestDistance;
	
	for (double	i = minCrd.x; size_t(i) <= size_t(maxCrd.x);)
	{
		for (size_t	j = minCrd.y; j <= maxCrd.y;)
		{
			for (size_t	k = minCrd.z; k <= maxCrd.z;)
			{
				bucketIdx = this->_CalcBucketIdx(i,j,k);
				shortestDistance = this->_CalcShortestDistance(t, i, j, k);
				if (shortestDistance < this->bucketFirst[bucketIdx].disFromWall)
				{
					this->bucketFirst[bucketIdx].disFromWall = shortestDistance;
				}
				k += BUCKET_LENGTH;
				bucketZ = k / BUCKET_LENGTH;
			}
			j += BUCKET_LENGTH;
			bucketY = j / BUCKET_LENGTH;
		}
		i += BUCKET_LENGTH;
		bucketX = i / BUCKET_LENGTH;
	}
}

void	PM::_CalcAllDistanceFromWall(const std::deque<Triangle>	&ts)
{
	for (size_t	i = 0; i < ts.size(); ++i)
	{
		this->_CalcDistanceFromWall(ts[i]);
	}
}

void	PM::SearchNeighborParticles(const size_t oneself)
{
	size_t	bucketX = this->ps[oneself].center.x / BUCKET_LENGTH;
	size_t	bucketY = this->ps[oneself].center.y / BUCKET_LENGTH;
	size_t	bucketZ = this->ps[oneself].center.z / BUCKET_LENGTH;
}

double	PM::W(const size_t i, const size_t oneself, bool gradientFlg) // weight
{
	double	rSQ = this->ps[i].center.Magnitude2d(this->ps[oneself].center);

	if (gradientFlg == GRADIENT)
	{
		return E_RADIUS / rSQ - rSQ / E_RADIUS;
	}

	return E_RADIUS / rSQ + rSQ / E_RADIUS - 2;
}

void	PM::PressureGradientTerm(Vec &p, const size_t oneself)
{

}

void	PM::ViscosityTerm(Vec &vi, const size_t oneself)
{
	double	kv; // Kinematic viscosity coefficient
	double	lambda;
	double	n0;

	for (size_t	i = 0; i < this->ps.size(); ++i)
	{
		if (i == oneself)
		{
			continue;
		}
		vi += (this->ps[i].velocity - this->ps[oneself].velocity) * 
			   this->W(i, oneself, GRADIENT);
	}

	vi *= kv * (2 * D / lambda * n0); 
}

void	PM::NavierStokesEquations(const	Vec &g)
{
	//PressureGradientTerm + ViscosityTerm + g
}

// PM::PM(const PM &PM)
// {
// 	*this = PM;
// }

// bool	PM::operator==(const PM &PM) const
// {
// 	return (this->x == PM.x) && 
// 		   (this->y == PM.y) &&
// 		   (this->r == PM.r);
// }

// PM&	PM::operator=(const PM &PM)
// {
// 	if (this != &PM)
// 	{
// 		this->x = PM.x;
// 		this->y = PM.y;
// 		this->r = PM.r;
// 	}
// 	return *this;
// }

// std::ostream &operator<<(std::ostream &ostrm, const PM &PM)
// {
// 	return ostrm << '(' << PM.x << ", " 
// 						<< PM.y << ", "
// 						<< PM.r << ')' 
// 						<< std::endl;
// }