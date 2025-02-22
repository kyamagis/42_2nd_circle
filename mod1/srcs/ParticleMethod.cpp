#include "../includes/ParticleMethod.hpp"
#include "../includes/MathUtils.hpp"

#define NUM_OF_PARTICLES 100
#define	D 3 // dimension number
#define	GRADIENT  0
#define LAPLACIAN 1
#define	BUCKET_LENGTH  E_RADIUS
#define	NUM_OF_BUCKETS
#define InvisibleSpace 2.0 * BUCKET_LENGTH

void	Delete(const void *n)
{
	if (n != NULL)
	{
		delete n;
	}
}

PM::PM()
{
	this->ps.resize(NUM_OF_PARTICLES);
}

PM::PM(const uint32_t	mapSize[2], 
	   const int64_t	maxHeight,
	   const std::deque<Triangle>	&ts):visibleMapSize(mapSize[X], mapSize[Y], maxHeight) ,
								bucketFirst(NULL),
								bucketNextIdxs(NULL)
{
	this->totalMapSize.x = mapSize[X] + 2.0 * InvisibleSpace;
	this->totalMapSize.y = mapSize[Y] + 2.0 * InvisibleSpace;
	this->totalMapSize.z = maxHeight;
	this->ps.resize(NUM_OF_PARTICLES);

	this->bucketRow    = this->totalMapSize.x / BUCKET_LENGTH;
	this->bucketColumn = this->totalMapSize.y / BUCKET_LENGTH;
	this->bucketDepth  = size_t(this->totalMapSize.z / BUCKET_LENGTH);
	this->numOfBuckets = this->bucketRow * this->bucketColumn * this->bucketDepth;

	this->InitBuckets();
	this->CalcAllDistanceFromWall(ts);
}

PM::~PM()
{
	Delete(this->bucketFirst);
	Delete(this->bucketNextIdxs);
}

void	PM::InitBuckets(void)
{
	this->bucketFirst       = new t_bucket[this->numOfBuckets];
	t_bucket	*bucketLast = new t_bucket[this->numOfBuckets];
	this->bucketNextIdxs    = new int64_t[NUM_OF_PARTICLES];

	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		this->bucketFirst[i].i   = -1;
		this->bucketFirst[i].disFromWall = 2.0 * BUCKET_LENGTH;
		bucketLast[i].i   = -1;
		bucketLast[i].disFromWall = 2.0 * BUCKET_LENGTH;
		if (i < NUM_OF_PARTICLES)
		{
			this->bucketNextIdxs[i] = -1;
		}
	}

	size_t	bucketX;
	size_t	bucketY;
	size_t	bucketZ;
	size_t	bucketIdx;
	size_t	nextIdx;	

	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		bucketX = this->ps[i].center.x / BUCKET_LENGTH;
		bucketY = this->ps[i].center.y / BUCKET_LENGTH;
		bucketZ = this->ps[i].center.z / BUCKET_LENGTH;
	
		bucketIdx = bucketX + 
					this->bucketColumn * bucketY + 
					this->bucketColumn * this->bucketDepth * bucketZ;
		nextIdx = bucketLast[bucketIdx].i;
		bucketLast[bucketIdx].i = i;
		if (nextIdx == -1)
		{
			this->bucketFirst[bucketIdx].i = i;
		}
		else
		{
			this->bucketNextIdxs[nextIdx] = i;
		}
	}
}




void	PM::CalcDistanceFromWall(const Triangle	&t)
{
	t.a.x == 
}

void	PM::CalcAllDistanceFromWall(const std::deque<Triangle>	&ts)
{
	for (size_t	i = 0; i < ts.size(); ++i)
	{
		this->CalcDistanceFromWall(ts[i]);
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