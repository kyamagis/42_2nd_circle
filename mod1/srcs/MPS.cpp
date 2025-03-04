#include "../includes/MPS.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Defines.hpp"


// MPS::MPS()
// {
// 	this->ps.resize(NUM_OF_PARTICLES);
// }

// totalMapSize(extend_map(mapSize[X]), extend_map(mapSize[Y]), extend_map(mapSize[Z]))



MPS::MPS(const uint32_t	mapSize[3], 
		const std::deque<Triangle> &ts)
		:BC(Vec(mapSize[X], mapSize[Y], mapSize[Z]), 
			Vec(mapSize[X], mapSize[Y], mapSize[Z])), 
		visibleMapSize(mapSize[X], mapSize[Y], mapSize[Z]) ,
		totalMapSize(this->visibleMapSize),
		g(0, 9.8, 0)
{
	init_wall_weight(this->_weights);
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

void	MPS::_InitTermCoefficient(void)
{
	double	n0 = 0;
	double	lambda = 0;

	double	tempLambda = 0;
	double	x;
	double	y;
	double	z;
	double	distance;
	double	distanceSQ;

	for (int32_t idxX = -4; idxX < 5; ++idxX) {
	for (int32_t idxY = -4; idxY < 5; ++idxY) {
	for (int32_t idxZ = -4; idxZ < 5; ++idxZ) {
		x =  I_DISTANCE * double(idxX);
		y =  I_DISTANCE * double(idxY);
		z =  I_DISTANCE * double(idxZ);
		distanceSQ = x*x + y*y + z*z;
		if (distanceSQ == 0.0)
		{
			continue;
		}
		if (distanceSQ <= E_RADIUS_SQ)
		{
			distance = sqrt(distanceSQ);
			n0 += WEIGHT(distance);
			tempLambda += distanceSQ * WEIGHT(distance);
		}
	}}}

	lambda =  tempLambda / n0;
	this->_cffVTerm  = 2.0 * KINEMATIC_VISCOSITY * D / n0 / lambda;
	this->_cffPress  = SPEED_OF_SOUND * SPEED_OF_SOUND / n0;
	this->_cffPGTerm = - D / n0;
}

void	MPS::_SetParameter(void)
{
	this->_InitTermCoefficient();
}

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

// double	MPS::W(const size_t i, const size_t oneself, bool gradientFlg) // weight
// {
// 	double	rSQ = this->ps[i].center.Magnitude2d(this->ps[oneself].center);

// 	if (gradientFlg == GRADIENT)
// 	{
// 		return E_RADIUS / rSQ - rSQ / E_RADIUS;
// 	}

// 	return E_RADIUS / rSQ + rSQ / E_RADIUS - 2;
// }

// void	MPS::PressureGradientTerm(Vec &p, const size_t oneself)
// {

// }

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

void	MPS::_CalcEachViscosity(const size_t oneself)
{
	size_t	currentBX = size_t(this->ps[oneself].center.x / BUCKET_LENGTH);
	size_t	currentBY = size_t(this->ps[oneself].center.y / BUCKET_LENGTH);
	size_t	currentBZ = size_t(this->ps[oneself].center.z / BUCKET_LENGTH);

	size_t	otherBX = _InitOtherBucketCoor(currentBX);
	size_t	otherBY = _InitOtherBucketCoor(currentBY);
	size_t	otherBZ = _InitOtherBucketCoor(currentBZ);

	size_t	maxBX = _InitMaxOtherBucketCoor(this->bucketRow,    currentBX);
	size_t	maxBY = _InitMaxOtherBucketCoor(this->bucketColumn, currentBY);
	size_t	maxBZ = _InitMaxOtherBucketCoor(this->bucketDepth,  currentBZ);

	size_t	bucketIdx;
	size_t	particleIdx;

	for (; otherBX <= maxBX ; ++otherBX){
	for (; otherBY <= maxBY ; ++otherBY){
	for (; otherBZ <= maxBZ ; ++otherBZ){
		bucketIdx = this->_CalcBucketIdx(otherBX, otherBY, otherBZ);
		particleIdx = this->bucketFirst[bucketIdx].firstPrtIdx;
		if (particleIdx == -1)
		{
			continue;
		}
		for (;;)
		{

			particleIdx = this->particleNextIdxs[particleIdx];
			if (particleIdx == -1)
			{
				break;
			}
		}
	}}}
}

void	MPS::ViscosityTerm(void)
{
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		this->_CalcEachViscosity(i);
	}
}

// void	MPS::NavierStokesEquations(const	Vec &g)
// {
// 	//PressureGradientTerm + ViscosityTerm + g
// }

// MPS::MPS(const MPS &mps): MPS
// {
// 	*this = MPS;
// }

// bool	MPS::operator==(const MPS &MPS) const
// {
// 	return (this->x == MPS.x) && 
// 		   (this->y == MPS.y) &&
// 		   (this->r == MPS.r);
// }

// MPS&	MPS::operator=(const MPS &mps)
// {
// 	if (this != &mps)
// 	{
// 		this->ps = mps.ps;
// 		this->visibleMapSize.x = mps.visibleMapSize.x;
// 		this->totalMapSize;
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