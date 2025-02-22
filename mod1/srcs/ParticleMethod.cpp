#include "../includes/ParticleMethod.hpp"
#include "../includes/MathUtils.hpp"

#define NUM_OF_PARTICLES 100
#define	D 3 // dimension number
#define	GRADIENT  0
#define LAPLACIAN 1
#define	BUCKET_LENGTH  E_RADIUS
#define	NUM_OF_BUCKETS
#define InvisibleSpace 2.0 * BUCKET_LENGTH

#define X 0
#define Y 1
#define Z 2

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

PM::PM(const uint32_t mapSize[2], const int64_t maxHeight):visibleMapSize(mapSize[X], mapSize[Y], maxHeight) ,
															bucketFirst(NULL),
															bucketLast(NULL),
															bucketNext(NULL)
{
	this->totalMapSize.x = mapSize[X] + 2.0 * InvisibleSpace;
	this->totalMapSize.y = mapSize[Y] + 2.0 * InvisibleSpace;
	this->totalMapSize.z = maxHeight;
	this->ps.resize(NUM_OF_PARTICLES);

	this->bucketRow = this->totalMapSize.x / BUCKET_LENGTH;
	this->bucketColumn = this->totalMapSize.y / BUCKET_LENGTH;
	this->bucketDepth = size_t(this->totalMapSize.z / BUCKET_LENGTH);
	this->numOfBuckets = this->bucketRow * this->bucketColumn * this->bucketDepth;

	this->InitBuckets();
}

PM::~PM()
{
	Delete(this->bucketFirst);
	Delete(this->bucketLast);
	Delete(this->bucketNext);
}

void	PM::InitBuckets(void)
{
	this->bucketFirst = new int64_t[this->numOfBuckets];
	this->bucketLast = new int64_t[this->numOfBuckets];
	this->bucketNext = new int64_t[NUM_OF_PARTICLES];

	for (size_t	i = 0; i < this->numOfBuckets; ++i)
	{
		this->bucketFirst[i] = -1;
		this->bucketLast[i] = -1;
		if (i < NUM_OF_PARTICLES)
		{
			this->bucketNext[i] = -1;
		}
	}

	size_t	bucketX;
	size_t	bucketY;
	size_t	bucketZ;
	size_t	bucketIdx;
	size_t	next;	

	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		bucketX = this->ps[i].center.x / BUCKET_LENGTH;
		bucketY = this->ps[i].center.y / BUCKET_LENGTH;
		bucketZ = this->ps[i].center.z / BUCKET_LENGTH;
	
		bucketIdx = bucketX + 
					this->bucketColumn * bucketY + 
					this->bucketColumn * this->bucketDepth * bucketZ;
		next = this->bucketNext[bucketIdx];
		this->bucketNext[bucketIdx] = i;
		if (next == -1)
		{
			this->bucketFirst[bucketIdx] = i;
		}
		else
		{
			this->bucketNext[next] = i;
		}
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