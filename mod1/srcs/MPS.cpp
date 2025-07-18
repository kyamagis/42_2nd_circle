#include <iomanip>
#include <iostream>
#include <fstream>

#include "../includes/MPS.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Defines.hpp"
#include "../includes/Print.hpp"

static size_t g_i;
static size_t g_o;
static Vec g_n;

// MPS::MPS()
// {
// 	this->ps.resize(NUM_OF_PARTICLES);
// }

// totalMapSize(extend_map(mapSize[X]), extend_map(mapSize[Y]), extend_map(mapSize[Z]))

MPS::MPS(const Vec	mapSize, const std::deque<Triangle> &ts)
		 :BC(Vec(mapSize), Vec(mapSize)), 
		 _g(0, 0, -9.8),
		visibleMapSize(mapSize) ,
		totalMapSize(this->visibleMapSize)
{
	this->_InitParticlesWaterColumnCollapse();
	this->_InitBuckets(ts);
	this->_SetParameter();
}

MPS::~MPS()
{

}

void	MPS::_InitBuckets(const std::deque<Triangle> &ts)
{
	this->BC_MakeBuckets(this->ps);
	this->BC_CalcAllDistanceFromWallSQ(ts);
}

void	MPS::_InitParticlesRain(void)
{
	
}

void	MPS::_InitParticlesWaterColumnCollapse(void)
{
	this->ps.resize(NUM_OF_PARTICLES);
	size_t	pIdx = 0;
	const double	initPos = BUCKET_LENGTH;
	const size_t	maxXIdx = (this->totalMapSize.x - initPos) / DIAMETER;
	const size_t	maxYIdx = (this->totalMapSize.y - initPos) / DIAMETER;
	const size_t	maxZIdx = (this->totalMapSize.z - initPos) / DIAMETER * (3.0 / 4.0);
	const size_t	psSize  = this->ps.size();
	double			x;
	double			y;

	for (size_t	yIdx = 0; yIdx < maxYIdx && pIdx < psSize; ++yIdx) 
	{
		y = RADIUS + yIdx * DIAMETER;
		for (size_t	xIdx = 0; xIdx < maxXIdx && pIdx < psSize; ++xIdx)
		{
			x = initPos + xIdx * DIAMETER;
			for (size_t	zIdx = 0; zIdx < maxZIdx && pIdx < psSize; ++zIdx)
			{
				ps[pIdx].center.x = x;
				ps[pIdx].center.y = y;
				ps[pIdx].center.z = RADIUS + zIdx * DIAMETER;
				ps[pIdx].r = RADIUS;
				ps[pIdx].rSQ = 0;
				ps[pIdx].color = gradation(initPos + (maxZIdx - 1) * DIAMETER, 
										   initPos, ps[pIdx].center.z);
				ps[pIdx].velocity = 0.0;
				ps[pIdx].acceleration = 0.0;
				ps[pIdx].pressure = 0.0;
				++pIdx;
			}
		}
	}
}

void	MPS::_InitTermCoefficient(void)
{
	double	lambda = 0;
	double	tempLambda = 0;

	Vec	r;
	double	distance;
	double	distanceSQ;

	for (int32_t idxX = -4; idxX < 5; ++idxX) {
	for (int32_t idxY = -4; idxY < 5; ++idxY) {
	for (int32_t idxZ = -4; idxZ < 5; ++idxZ) {
		r.x =  I_DISTANCE * double(idxX);
		r.y =  I_DISTANCE * double(idxY);
		r.z =  I_DISTANCE * double(idxZ);
		distanceSQ = r.MagnitudeSQ3d();
		if (distanceSQ == 0.0)
		{
			continue;
		}
		if (distanceSQ <= this->bc_radius_effectiveSQ)
		{
			distance = sqrt(distanceSQ);
			this->_n0 += WEIGHT(distance);
			tempLambda += distanceSQ * WEIGHT(distance);
		}
	}}}

	lambda =  tempLambda / this->_n0;
	this->_cffVTerm  = 2.0 * KINEMATIC_VISCOSITY * D / this->_n0 / lambda;
	this->_cffPress  = SPEED_OF_SOUND * SPEED_OF_SOUND / this->_n0;
	this->_cffPGTerm = - D / this->_n0;
}

void	MPS::_SetParameter(void)
{
	this->_InitTermCoefficient();
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
	if (coor < max + 1)
	{
		return  coor + 1;
	}
	return coor;
}

double	MPS::_WallWeight(const double disFromWall)
{
	if (this->BC_IsOutOfWallWeightRange(disFromWall))
	{
		return 0.0;
	}
	return this->BC_InterpolateWallWeight(disFromWall);
}

void	MPS::_CalcOneOnOneViscosity(const Vec &oneselfVel, 
									Vec &acceleration, 
									const size_t particleIdx,
									const double distanceSQP)
{
	if (distanceSQP < this->bc_radius_effectiveSQ) 
	{
		const double	distanceP = sqrt(distanceSQP);
		const double	w = WEIGHT(calibrate_dist(distanceP));
		acceleration += (this->ps[particleIdx].velocity - oneselfVel) * w;
	}
}

void	MPS::_CalcOneOnOneCollision(const Vec &oneselfVel,
									Vec &acceleration, 
									const size_t particleIdx,
									const Vec &dr,
									const double distanceSQP)
{
	if (distanceSQP < DISTANCE_LIMIT_SQ)
	{
		double	closing = (this->ps[particleIdx].velocity - oneselfVel).DotProduct3d(dr);

		if (closing < 0.0)
		{
			closing *= REPULSION_COEFFICIENT / (2 * calibrate_dist(distanceSQP));
			acceleration += dr * closing; // KOKO
		}
	}
}

void	MPS::_CalcOneOnOnePressure(const double distanceSQP, double &ni)
{
	if (distanceSQP < this->bc_radius_effectiveSQ)
	{
		const double	distance = sqrt(distanceSQP);
		const double	w = WEIGHT(calibrate_dist(distance));
		ni += w;
	}
}

void	MPS::_SmallerPressure(double &minPressure, const size_t particleIdx, 
								const double distanceSQP)
{
	if (distanceSQP < this->bc_radius_effectiveSQ)
	{
		if(this->ps[particleIdx].pressure < minPressure)
		{
			minPressure = this->ps[particleIdx].pressure;
		}
	}
}

void	MPS::_CalcOneOnOnePressureGradient(double &minPressure,
											Vec &acceleration, 
											const size_t particleIdx,
											const Vec &dr,
											const double distanceSQP)
{
	if (distanceSQP < this->bc_radius_effectiveSQ)
	{
		const double distance = sqrt(distanceSQP);
		double w = WEIGHT(distance);
		w *= (this->ps[particleIdx].pressure - minPressure) / distanceSQP;
		acceleration += dr * w;
	}
}

void	MPS::_SwitchOperation(const e_operation e, 
								double &minPressure,
								const Vec &oneselfPos,
								const Vec &oneselfVel, 
								Vec &acceleration, 
								const size_t particleIdx,
								double &ni)
{
	const Vec		dr = this->ps[particleIdx].center - oneselfPos;
	const double	distanceSQP = dr.MagnitudeSQ3d();

	switch (e)
	{
		case e_VISCOSITY:
			this->_CalcOneOnOneViscosity(oneselfVel, acceleration, 
										particleIdx, distanceSQP);
			break;
		case e_COLLISION:
			this->_CalcOneOnOneCollision(oneselfVel, acceleration, 
										particleIdx, dr, distanceSQP);
			break;
		case e_PRESSURE:
			this->_CalcOneOnOnePressure(distanceSQP, ni);
			break;
		case e_PGRADIENT1:
			this->_SmallerPressure(minPressure, particleIdx, distanceSQP);
			break;
		case e_PGRADIENT2:
			this->_CalcOneOnOnePressureGradient(minPressure, acceleration, 
												particleIdx, dr, distanceSQP);
			break;
		default:
			break;
	}
}

void	MPS::_SwitchContributionFromWall(const size_t oneself, const e_operation e,
										 const size_t currentBX,
										 const size_t currentBY,
										 const size_t currentBZ, 
										 Vec &acceleration)
{
	const size_t	bucketIdx = this->BC_CalcBucketIdx(currentBX, currentBY, currentBZ);
	const double	distFromWall = 
	trilinear_interpolation_dist(this->ps[oneself].center, currentBX, currentBY, currentBZ, 
								 this->buckets[bucketIdx].distFromWalls);
	double	wallWeight;
	Vec		nVector;
	switch (e)
	{
		case e_VISCOSITY:
			this->ps[oneself].bucketX = currentBX;
			this->ps[oneself].bucketY = currentBY;
			this->ps[oneself].bucketZ = currentBZ;
			if (distFromWall < DIAMETER)
			{
				wallWeight = this->_WallWeight(distFromWall);
				if (0.0 < wallWeight)
				{
					acceleration += (this->ps[oneself].velocity * -2.0) * wallWeight;
				}
			}
			break;
		case e_COLLISION:
			if (distFromWall < DIAMETER)
			{
				nVector = this->buckets[bucketIdx].nInterpolation;
				double	closing = this->ps[oneself].velocity.DotProduct3d(nVector);
				if (closing < 0.0 && 0.0 < distFromWall)
				{
					acceleration += nVector * REPULSION_COEFFICIENT * (-closing);
				}
			}
			break;
		case e_PRESSURE:
			break;
		case e_PGRADIENT2:
			if (distFromWall < RADIUS)
			{
				nVector = this->buckets[bucketIdx].nInterpolation;
				acceleration -= nVector * DENSITY_OF_PARTICLES * (RADIUS - distFromWall);
			}
			break;
		default:
			break;
	}
}

void	MPS::_SwitchAssignmentOfAcceleration(const size_t oneself, const e_operation e, 
											 const Vec &acceleration, const double ni)
{
	switch (e)
	{
		case e_VISCOSITY:
			this->ps[oneself].acceleration = acceleration * this->_cffVTerm;
			break;
		case e_COLLISION:
			this->ps[oneself].acceleration = acceleration;
			break;
		case e_PRESSURE:
			this->ps[oneself].pressure = (this->_n0 < ni) * (ni - this->_n0) * 
										  this->_cffPress * DENSITY_OF_PARTICLES;
			break;
		case e_PGRADIENT2:
			this->ps[oneself].acceleration = acceleration * 1 / DENSITY_OF_PARTICLES * this->_cffPGTerm;
			break;
		default:
			break;
	}
}

void	MPS::_SearchNeighborParticles(const size_t oneself, const e_operation e, double &minPressure)
{
	g_i = oneself;
	const size_t	currentBX = size_t(this->ps[oneself].center.x / this->bc_bucketLength);
	const size_t	currentBY = size_t(this->ps[oneself].center.y / this->bc_bucketLength);
	const size_t	currentBZ = size_t(this->ps[oneself].center.z / this->bc_bucketLength);

	const Vec	oneselfPos(this->ps[oneself].center);
	const Vec	oneselfVel(this->ps[oneself].velocity);

	Vec	acceleration(0.0);

	if (e == e_COLLISION)
	{
		acceleration = oneselfVel;
	}

	const size_t	otherBEdgeX = _InitOtherBucketCoor(currentBX);
	const size_t	otherBEdgeY = _InitOtherBucketCoor(currentBY);
	const size_t	otherBEdgeZ = _InitOtherBucketCoor(currentBZ);

	const size_t	maxBX = _InitMaxOtherBucketCoor(this->bucketRow,    currentBX);
	const size_t	maxBY = _InitMaxOtherBucketCoor(this->bucketColumn, currentBY);
	const size_t	maxBZ = _InitMaxOtherBucketCoor(this->bucketDepth,  currentBZ);

	size_t	bucketIdx;
	size_t	particleIdx;

	double	ni = 0.0;

	for (size_t	otherBX = otherBEdgeX; otherBX <= maxBX ; ++otherBX){
	for (size_t	otherBY = otherBEdgeY; otherBY <= maxBY ; ++otherBY){
	for (size_t	otherBZ = otherBEdgeZ; otherBZ <= maxBZ ; ++otherBZ){
		bucketIdx   = this->BC_CalcBucketIdx(otherBX, otherBY, otherBZ);
		particleIdx = this->buckets[bucketIdx].firstPrtIdx;		
		if (particleIdx == UINT64_MAX)
		{
			continue;
		}
		while (true)
		{
			if (particleIdx != oneself && this->ps[particleIdx].validFlag) 
			{
				g_o = particleIdx;
				this->_SwitchOperation(e, minPressure, oneselfPos, oneselfVel, 
									   acceleration,  particleIdx, ni);
			}
			particleIdx = this->particleNextIdxs[particleIdx];
			if (particleIdx == UINT64_MAX)
			{
				break;
			}
		}
	}}}
	this->_SwitchContributionFromWall(oneself, e, currentBX, currentBY, currentBZ, 
									  acceleration);
	this->_SwitchAssignmentOfAcceleration(oneself, e, acceleration, ni);
}

bool	MPS::_CheckOutOfRange(const Vec &pos)
{
	if (pos.x < EPS || 
		pos.y < EPS || 
		pos.z < EPS)
	{
		return false;
	}
	if (this->visibleMapSize.x - 1 < pos.x || 
		this->visibleMapSize.y - 1 < pos.y || 
		this->visibleMapSize.z - 1 < pos.z)
	{
		return false;
	}
	return true;
}

void	MPS::_ViscosityAndGravityTerm(void)
{
	double	noMeaning = 0;
	#pragma omp parallel for schedule(dynamic, 64)

	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		if (this->ps[i].validFlag)
		{
			this->_SearchNeighborParticles(i, e_VISCOSITY, noMeaning);
			this->ps[i].acceleration += this->_g;
		}
	}
}

void	MPS::_UpdateVPA1(void)
{
	#pragma omp parallel for

	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		if (this->ps[i].validFlag)
		{
			this->ps[i].velocity += this->ps[i].acceleration * DELTA_TIME;
			this->ps[i].center   += this->ps[i].velocity * DELTA_TIME;
			
			this->ps[i].validFlag = this->_CheckOutOfRange(this->ps[i].center);
			this->ps[i].acceleration = 0.0;
		}
	}
}

void	MPS::_CalcParticlesCollision(void)
{
	double	noMeaning = 0;
	#pragma omp parallel for schedule(dynamic, 64)
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		if (this->ps[i].validFlag)
		{
			this->_SearchNeighborParticles(i, e_COLLISION, noMeaning);
		}
	}
	#pragma omp parallel for
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		if (this->ps[i].validFlag)
		{
			this->ps[i].velocity = this->ps[i].acceleration;
		}
	}
}

void	MPS::_CalcParticlesPressure(void)
{
	double	noMeaning = 0;
	#pragma omp parallel for schedule(dynamic, 64)
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		if (this->ps[i].validFlag)
		{
			this->_SearchNeighborParticles(i, e_PRESSURE, noMeaning);
		}
	}
}

void	MPS::_PressureGradientTerm(void)
{
	double	minPressure;
	#pragma omp parallel for schedule(dynamic, 64)

	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		if (this->ps[i].validFlag)
		{
			minPressure = this->ps[i].pressure;
			this->_SearchNeighborParticles(i, e_PGRADIENT1, minPressure);
			this->_SearchNeighborParticles(i, e_PGRADIENT2, minPressure);
		}
	}
}

void	MPS::_UpdateVPA2(void)
{
	#pragma omp parallel for

	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		if (this->ps[i].validFlag)
		{
			this->ps[i].velocity += this->ps[i].acceleration * DELTA_TIME;
			this->ps[i].center   += this->ps[i].acceleration * DELTA_TIME * DELTA_TIME;

			this->ps[i].validFlag = this->_CheckOutOfRange(this->ps[i].center);
			this->ps[i].acceleration = 0.0;
		}
	}
}

void	MPS::_UpdateVPA3(void)
{
	#pragma omp parallel for

	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		if (this->ps[i].validFlag)
		{
			this->ps[i].center += this->ps[i].velocity * DELTA_TIME;

			this->ps[i].validFlag = this->_CheckOutOfRange(this->ps[i].center);
			this->ps[i].acceleration = 0.0;
		}
	}
}

void	MPS::_NavierStokesEquations(void)
{
	this->BC_UpdateBuckets(this->ps);
	this->_ViscosityAndGravityTerm();
	this->_UpdateVPA1();
	this->_CalcParticlesCollision();
	this->_CalcParticlesPressure();
	this->_PressureGradientTerm();
	this->_UpdateVPA2();
}

void	MPS::Simulation(t_log logs[SIMULATION_TIME])
{
	const double maxTime = ONE_SECOUD;

	for (size_t elapsedTime = 0; elapsedTime < SIMULATION_TIME; elapsedTime += maxTime)
	{
		logs[elapsedTime].ps = this->ps;
		for (double time = 0.0; time < maxTime; time += DELTA_TIME) {
			this->_NavierStokesEquations();
		}
		std::cout << elapsedTime << " / " << SIMULATION_TIME
		<< "\r" << std::flush;
	}
	std::cout <<  std::endl;
}
