#include "../includes/MPS.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Defines.hpp"
#include <iostream>
#include <fstream>

// MPS::MPS()
// {
// 	this->ps.resize(NUM_OF_PARTICLES);
// }

// totalMapSize(extend_map(mapSize[X]), extend_map(mapSize[Y]), extend_map(mapSize[Z]))

void	_InitWallWeight(void)
{
	std::deque<Particle>	ps;
	Particle				point(0,RADIUS,0);

	double	x;
	double	y;
	double	z;
	double	distance;
	double	distanceSQ;

	for (int32_t idxX = -4; idxX <= 0; ++idxX) {
	for (int32_t idxY = -4; idxY <= 0; ++idxY) {
	for (int32_t idxZ = -4; idxZ <= 0; ++idxZ) {
		x =  I_DISTANCE * double(idxX);
		y =  I_DISTANCE * double(idxY);
		z =  I_DISTANCE * double(idxZ);
		ps.push_back(Particle(x, y, z));
	}}}

	std::ofstream outputfile("./WallWeight");
	std::deque<Particle>::iterator	it;
	const double	diff = 0.1;
	double			weight;

	while (0 < ps.size())
	{
		it = ps.begin();
		weight = 0.0;
		while (it != ps.end())
		{
			distanceSQ = (it)->center.MagnitudeSQ3d(point.center);
			if (distanceSQ <= E_RADIUS_SQ)
			{
				distance = sqrt(distanceSQ);
				weight += WEIGHT(distance);
			}
			else
			{
				it = ps.erase(it);
			}
		}
		outputfile << point.center.y - RADIUS << ", " << weight << std::endl;
		point.center.y += diff;
	}
	outputfile.close();
}

MPS::MPS(const uint32_t	mapSize[3], 
		const std::deque<Triangle> &ts)
		:BC(Vec(mapSize[X], mapSize[Y], mapSize[Z]), 
			Vec(mapSize[X], mapSize[Y], mapSize[Z])), 
		visibleMapSize(mapSize[X], mapSize[Y], mapSize[Z]) ,
		totalMapSize(this->visibleMapSize)
{
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



void	MPS::_IniDensityAndLambda(void)
{
	this->_n0 = 0;
	// this->_lambda = 0;

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
			this->_n0 += WEIGHT(distance);
			tempLambda += distance;
		}
	}}}

	this->_lambda = this->_n0 / tempLambda;
}

void	SetParameter(void)
{
	
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

// void	MPS::ViscosityTerm(Vec &vi, const size_t oneself)
// {
// 	double	kv; // Kinematic viscosity coefficient
// 	double	lambda;
// 	double	n0;

// 	for (size_t	i = 0; i < this->ps.size(); ++i)
// 	{
// 		if (i == oneself)
// 		{
// 			continue;
// 		}
// 		vi += (this->ps[i].velocity - this->ps[oneself].velocity) * 
// 			   this->W(i, oneself, GRADIENT);
// 	}

// 	vi *= kv * (2 * D / lambda * n0); 
// }

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