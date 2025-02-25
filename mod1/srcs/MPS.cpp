#include "../includes/MPS.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Defines.hpp"

// MPS::MPS()
// {
// 	this->ps.resize(NUM_OF_PARTICLES);
// }

// totalMapSize(extend_map(mapSize[X]), extend_map(mapSize[Y]), extend_map(mapSize[Z]))

MPS::MPS(const uint32_t	mapSize[3], 
	     const int64_t	maxHeight,
	     const std::deque<Triangle> &ts)
	   :visibleMapSize(mapSize[X], mapSize[Y], mapSize[Z]) ,
	   	totalMapSize(this->visibleMapSize),
	    BC(this->visibleMapSize, this->totalMapSize) 
{
	(void)maxHeight;
	this->_InitParticlesWaterColumnCollapse();
	this->_InitBuckets(ts);
}

MPS::~MPS()
{

}

void	MPS::_InitBuckets(const std::deque<Triangle> &ts)
{
	this->_MakeBuckets(this->ps);
	this->_CalcAllDistanceFromWall(ts);
}

void	MPS::_InitParticlesWaterColumnCollapse(void)
{
	this->ps.resize(NUM_OF_PARTICLES);
	size_t	pIdx = 0;
	size_t	maxXIdx = this->totalMapSize.x / DIAMETER;
	size_t	maxYIdx = this->totalMapSize.y / DIAMETER;
	size_t	maxZIdx = this->totalMapSize.z / DIAMETER;

	size_t	psSize = this->ps.size();

	for (size_t	yIdx = 0; yIdx < maxYIdx && pIdx < psSize; ++yIdx, ++pIdx)
	{
		ps[pIdx].center.y = RADIUS + yIdx * DIAMETER;
		for (size_t	xIdx = 0; xIdx < maxXIdx && pIdx < psSize; ++xIdx, ++pIdx)
		{
			ps[pIdx].center.x = RADIUS + xIdx * DIAMETER;
			for (size_t	zIdx = 0; zIdx < maxZIdx && pIdx < psSize; ++zIdx, ++pIdx)
			{
				ps[pIdx].center.z = RADIUS + zIdx * DIAMETER;
				ps[pIdx].r = RADIUS;
				ps[pIdx].velocity = 0.0;
			}
		}
	}
}



// void	MPS::_SearchNeighborParticle(const size_t i)
// {
// 	size_t	currentBX = size_t(this->ps[i].center.x / BUCKET_LENGTH);
// 	size_t	currentBY = size_t(this->ps[i].center.y / BUCKET_LENGTH);
// 	size_t	currentBZ = size_t(this->ps[i].center.z / BUCKET_LENGTH);

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
// 		particleIdx = this->bucketFirst[bucketIdx].firstPrtIdx;
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

// void	MPS::_SearchNeighborParticles(const size_t oneself)
// {
// 	// size_t	bucketX = this->ps[oneself].center.x / BUCKET_LENGTH;
// 	// size_t	bucketY = this->ps[oneself].center.y / BUCKET_LENGTH;
// 	// size_t	bucketZ = this->ps[oneself].center.z / BUCKET_LENGTH;

// 	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
// 	{
// 		this->_SearchNeighborParticle(i);

// 	}
// }

double	MPS::W(const size_t i, const size_t oneself, bool gradientFlg) // weight
{
	double	rSQ = this->ps[i].center.Magnitude2d(this->ps[oneself].center);

	if (gradientFlg == GRADIENT)
	{
		return E_RADIUS / rSQ - rSQ / E_RADIUS;
	}

	return E_RADIUS / rSQ + rSQ / E_RADIUS - 2;
}

void	MPS::PressureGradientTerm(Vec &p, const size_t oneself)
{

}

void	MPS::ViscosityTerm(Vec &vi, const size_t oneself)
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

void	MPS::NavierStokesEquations(const	Vec &g)
{
	//PressureGradientTerm + ViscosityTerm + g
}

// MPS::MPS(const MPS &MPS)
// {
// 	*this = MPS;
// }

// bool	MPS::operator==(const MPS &MPS) const
// {
// 	return (this->x == MPS.x) && 
// 		   (this->y == MPS.y) &&
// 		   (this->r == MPS.r);
// }

// MPS&	MPS::operator=(const MPS &MPS)
// {
// 	if (this != &MPS)
// 	{
// 		this->x = MPS.x;
// 		this->y = MPS.y;
// 		this->r = MPS.r;
// 	}
// 	return *this;
// }

// std::ostream &operator<<(std::ostream &ostrm, const MPS &MPS)
// {
// 	return ostrm << '(' << MPS.x << ", " 
// 						<< MPS.y << ", "
// 						<< MPS.r << ')' 
// 						<< std::endl;
// }