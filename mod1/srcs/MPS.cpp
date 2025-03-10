#include "../includes/MPS.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Defines.hpp"
#include "../includes/Print.hpp"


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
		g(0, 0, -9.8)
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
	this->_MakeBuckets(this->ps);
	this->_CalcAllDistanceFromWall(ts);
}

void	MPS::_InitParticlesWaterColumnCollapse(void)
{
	this->ps.resize(NUM_OF_PARTICLES);
	size_t	pIdx = 0;
	size_t	maxXIdx = this->totalMapSize.x / DIAMETER;
	size_t	maxYIdx = this->totalMapSize.y / DIAMETER;
	size_t	maxZIdx = (this->totalMapSize.z / DIAMETER);

	// std::cout << maxZIdx << std::endl;

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
		if (distanceSQ <= E_RADIUS_SQ)
		{
			distance = sqrt(distanceSQ);
			this->n0 += WEIGHT(distance);
			tempLambda += distanceSQ * WEIGHT(distance);
		}
	}}}

	lambda =  tempLambda / this->n0;
	this->_cffVTerm  = 2.0 * KINEMATIC_VISCOSITY * D / this->n0 / lambda;
	this->_cffPress  = SPEED_OF_SOUND * SPEED_OF_SOUND / this->n0;
	this->_cffPGTerm = - D / this->n0;
}

void	MPS::_SetParameter(void)
{
	this->_InitTermCoefficient();
}

// double	MPS::W(const size_t i, const size_t oneself, bool gradientFlg) // weight
// {
// 	double	rSQ = this->ps[i].center.Magnitude2d(this->ps[oneself].center);

// 	if (gradientFlg == GRADIENT)
// 	{
// 		return E_RADIUS / rSQ - rSQ / E_RADIUS;
// 	}

// 	return E_RADIUS / rSQ + rSQ / E_RADIUS - 2;
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
	if (coor < max + 1)
	{
		return  coor + 1;
	}
	return coor;
}

void	MPS::_CalcOneOnOneViscosity(const Vec &oneselfVel, 
									Vec &acceleration, 
									const size_t bucketIdx, 
									const size_t particleIdx,
									const double distanceSQP)
{
	double	distanceP;
	double	wallWeight;
	double	w;

	if (distanceSQP < E_RADIUS_SQ) 
	{
		if (distanceSQP <= 0.0)
		{
			std::cout << "distanceSQP" <<" " << distanceSQP << std::endl;
		}
		distanceP = sqrt(distanceSQP);
		w = WEIGHT(distanceP);
		acceleration += (this->ps[particleIdx].velocity - oneselfVel) * w;
	}
	wallWeight  = this->buckets[bucketIdx].wallWeight;
	if (0.0 < wallWeight)
	{
		acceleration += (oneselfVel * -2.0) * wallWeight;
	}
}

void	MPS::_CalcOneOnOneCollision(const Vec &oneselfPos,
									const Vec &oneselfVel,
									Vec &acceleration, 
									const size_t particleIdx,
									const Vec &dr,
									const double distanceSQP)
{
	(void)oneselfPos;
	if (distanceSQP < DISTANCE_LIMIT_SQ)
	{
		double	closing = (this->ps[particleIdx].velocity - oneselfVel).DotProduct3d(dr);

		if (closing < 0.0)
		{
			closing *= REPULSION_COEFFICIENT / 2 * distanceSQP;
			acceleration += dr * closing;	
		}
	}
}

void	MPS::_CalcOneOnOnePressure(const double distanceSQP, double &ni)
{
	if (distanceSQP < E_RADIUS_SQ)
	{
		double distance = sqrt(distanceSQP);
		double w = WEIGHT(distance);
		ni += w;
	}
}

void	MPS::_SmallerPressure(double &minPressure, const size_t particleIdx, const double distanceSQP)
{
	if (distanceSQP < E_RADIUS_SQ)
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
	if (distanceSQP < E_RADIUS_SQ)
	{
		double distance = sqrt(distanceSQP);
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
								const size_t bucketIdx, 
								const size_t particleIdx,
								double &ni)
{
	const Vec		dr = this->ps[particleIdx].center - oneselfPos;
	const double	distanceSQP = dr.MagnitudeSQ3d();

	switch (e)
	{
		case e_VISCOSITY:
			this->_CalcOneOnOneViscosity(oneselfVel, acceleration, 
										bucketIdx, particleIdx,	distanceSQP);
			break;
		case e_COLLISION:
			this->_CalcOneOnOneCollision(oneselfPos, oneselfVel, acceleration, 
										particleIdx, dr, distanceSQP);
			break;
		case e_PRESSURE:
			this->_CalcOneOnOnePressure(distanceSQP, ni);
			break;
		case e_PGRADIENT1:
			this->_SmallerPressure(minPressure, particleIdx, distanceSQP);
			break;
		case e_PGRADIENT2:
			break;
			this->_CalcOneOnOnePressureGradient(minPressure, acceleration, 
												particleIdx, dr, distanceSQP);
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
			this->ps[oneself].pressure = (this->n0 < ni) * (ni - this->n0) * 
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
	const size_t	currentBX = size_t(this->ps[oneself].center.x / BUCKET_LENGTH);
	const size_t	currentBY = size_t(this->ps[oneself].center.y / BUCKET_LENGTH);
	const size_t	currentBZ = size_t(this->ps[oneself].center.z / BUCKET_LENGTH);

	const Vec	oneselfPos(this->ps[oneself].center);
	const Vec	oneselfVel(this->ps[oneself].velocity);
	Vec	acceleration;

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
		bucketIdx   = this->_CalcBucketIdx(otherBX, otherBY, otherBZ);
		particleIdx = this->buckets[bucketIdx].firstPrtIdx;
		if (particleIdx == UINT64_MAX)
		{
			continue;
		}
		while (true)
		{
			if (particleIdx != oneself && this->ps[particleIdx].validFlag) 
			{
				
				this->_SwitchOperation(e, minPressure, oneselfPos, oneselfVel, 
									   acceleration, bucketIdx, particleIdx, ni);
			}
			particleIdx = this->particleNextIdxs[particleIdx];
			if (particleIdx == UINT64_MAX)
			{
				break;
			}
		}
	}}}
	this->_SwitchAssignmentOfAcceleration(oneself, e, acceleration, ni);
}

void	MPS::_UpdateVPA1(void)
{
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		if (!this->ps[i].validFlag)
		{
			continue;
		}
		this->ps[i].velocity += this->ps[i].acceleration * DELTA_TIME;
		this->ps[i].center += this->ps[i].velocity * DELTA_TIME;

		if (this->ps[i].center.x < 0.0 || 
			this->ps[i].center.y < 0.0 || 
			this->ps[i].center.z < 0.0)
		{
			this->ps[i].validFlag = false;
		}

		this->ps[i].acceleration = 0.0;
	}
	// std::cout << this->ps[0] << std::endl;
}

void	MPS::_UpdateVPA2(void)
{
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		if (!this->ps[i].validFlag)
		{
			continue;
		}
		this->ps[i].velocity += this->ps[i].acceleration * DELTA_TIME;
		this->ps[i].center += this->ps[i].acceleration * DELTA_TIME * DELTA_TIME;

		if (this->ps[i].center.x < 0.0 || 
			this->ps[i].center.y < 0.0 || 
			this->ps[i].center.z < 0.0)
		{
			this->ps[i].validFlag = false;
		}

		this->ps[i].acceleration = 0.0;
	}
	// std::cout << this->ps[0] << std::endl;
}

void	MPS::_ViscosityAndGravityTerm(void)
{
	double	noMeaning = 0;
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		if (this->ps[i].validFlag)
		{
			this->_SearchNeighborParticles(i, e_VISCOSITY, noMeaning);
			this->ps[i].acceleration += this->g;
		}
	}
}

void	MPS::_CalcParticlesCollision(void)
{
	double	noMeaning = 0;
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		if (this->ps[i].validFlag)
		{
			this->_SearchNeighborParticles(i, e_COLLISION, noMeaning);
		}
	}
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
	double	minPressure = 0;
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

void	MPS::NavierStokesEquations(void)
{
	this->_UpdateBuckets(this->ps);
	this->_ViscosityAndGravityTerm();
	this->_UpdateVPA1();
	this->_CalcParticlesCollision();
	this->_CalcParticlesPressure();
	this->_PressureGradientTerm();
	this->_UpdateVPA2();
	// this->_CalcParticlesPressure();
	// for (size_t i = 0; i < NUM_OF_PARTICLES; ++i)
	// {
	// 	this->pressureLog[i] += this->ps[i].pressure;
	// }
}

void	MPS::DrawParticles(const Vec &halfMapSize, const double midHeight)
{
	glPointSize(2.0f);
	glBegin(GL_POINTS);
	glColor3f(0.5, 0.5, 1);
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		drawVertex(move_vec_to_map_center(this->ps[i].center, halfMapSize, midHeight));
	}
	glEnd();
}

// MPS::MPS(const MPS &mps): MPS
// {
// 	*this = MPS;
// }

// bool	MPS::operator==(const MPS &mps) const
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

// std::ostream &operator<<(std::ostream &ostrm, const MPS &mps)
// {
// 	return ostrm << '(' << MPS.x << ", " 
// 						<< MPS.y << ", "
// 						<< MPS.r << ')' 
// 						<< std::endl;
// }
