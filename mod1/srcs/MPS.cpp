#include <iomanip>
#include "../includes/MPS.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Defines.hpp"
#include "../includes/Print.hpp"

static size_t g_i;
static size_t g_o;

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
	this->BC_MakeBuckets(this->ps);
	this->BC_CalcAllDistanceFromWallSQ(ts);
}

void	MPS::_InitParticlesWaterColumnCollapse(void)
{
	this->ps.resize(NUM_OF_PARTICLES);
	size_t	pIdx = 0;
	const size_t	maxXIdx = this->totalMapSize.x / DIAMETER;
	const size_t	maxYIdx = this->totalMapSize.y / DIAMETER;
	const size_t	maxZIdx = (this->totalMapSize.z / DIAMETER) / 2;
	const size_t	psSize  = this->ps.size();
	const double	initPos = DIAMETER * 2.0;
	// const double	radius  = RADIUS * 0.7;
	const double	radius  = RADIUS;
	const double	diameter  =  radius * 2.0;
	double			x;
	double			y;

	// Print::OutWords("particle", maxXIdx, maxYIdx, maxZIdx, psSize);

	for (size_t	yIdx = 0; yIdx < maxYIdx && pIdx < psSize; ++yIdx) 
	{
		y = radius + yIdx * diameter;
		for (size_t	xIdx = 0; xIdx < maxXIdx && pIdx < psSize; ++xIdx)
		{
			x = radius + xIdx * diameter;
			for (size_t	zIdx = 0; zIdx < maxZIdx && pIdx < psSize; ++zIdx)
			{
				ps[pIdx].center.x = x;
				ps[pIdx].center.y = y;
				ps[pIdx].center.z = initPos + zIdx * diameter;
				ps[pIdx].r = RADIUS;
				ps[pIdx].color = gradation(initPos + (maxZIdx - 1) * diameter, 
										   initPos, ps[pIdx].center.z);
				ps[pIdx].velocity = 0.0;
				ps[pIdx].acceleration = 0.0;
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
	if (disFromWall < 0.0)
	{
		Print::Err("_WallWeight: disFromWall");
	}
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
	if (distanceSQP < E_RADIUS_SQ) 
	{
		double	distanceP = sqrt(distanceSQP);
		double	w;
		if (distanceP < 0.1)
		{
			// Print::OutWords(particleIdx, distanceP);
			distanceP = 0.1;
		}
		w = WEIGHT(distanceP);
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
			if (distanceSQP < 0.01)
			{
				closing *= REPULSION_COEFFICIENT / (2 * 0.01);
			}
			else
			{
				closing *= REPULSION_COEFFICIENT / (2 * distanceSQP);
			}
			acceleration += dr * closing; // KOKO
		}
	}
}

void	MPS::_CalcOneOnOnePressure(const double distanceSQP, double &ni)
{
	if (distanceSQP < E_RADIUS_SQ)
	{
		double distance = sqrt(distanceSQP);
		if (distance < 0.1)
		{
			distance = 0.1;
		}
		const double w = WEIGHT(distance);
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
			break;
			this->_CalcOneOnOnePressureGradient(minPressure, acceleration, 
												particleIdx, dr, distanceSQP);
		default:
			break;
	}
}

void	MPS::_SwitchContributionFromWall(const size_t oneself, const e_operation e,
										 const size_t currentBX,
										 const size_t currentBY,
										 const size_t currentBZ, 
										 Vec &acceleration, double &ni)
{
	const size_t	currentBIdx = this->BC_CalcBucketIdx(currentBX, currentBY, currentBZ);
	double	distFromWallSQ = 
	this->BC_InterpolateDistFromWallSQ(this->ps[oneself].center, currentBX, currentBY, currentBZ);
	double	distFromWall;
	double	wallWeight;
	Vec		nVector;
	double	closing;
	switch (e)
	{
		case e_VISCOSITY:
			if (distFromWallSQ < E_RADIUS_SQ)
			{
				wallWeight = this->_WallWeight(sqrt(distFromWallSQ));
				if (0.0 < wallWeight)
				{
					acceleration += (this->ps[oneself].velocity * -2.0) * wallWeight;
				}
			}
			break;
		case e_COLLISION:
			if (distFromWallSQ < DISTANCE_LIMIT_SQ)
			{
				nVector = this->buckets[currentBIdx].n;
				closing = this->ps[oneself].velocity.DotProduct3d(nVector);

				// if (oneself == 0)
				// {
				// 	Print::OutWords(oneself, this->ps[oneself].velocity, nVector, closing);
				// }
				if (closing < 0.0)
				{
					// Print::OutWords(oneself, this->ps[oneself].velocity, nVector, closing);
					acceleration += nVector * REPULSION_COEFFICIENT * (-closing);
				}
			}
			break;
		case e_PRESSURE:
			if (distFromWallSQ < E_RADIUS_SQ)
			{
				ni += this->_WallWeight(sqrt(distFromWallSQ));
			}
			break;
		case e_PGRADIENT2:
			if (distFromWallSQ < E_RADIUS_SQ)
			{
				distFromWall = sqrt(distFromWallSQ);
				wallWeight   = this->_WallWeight(distFromWall);
				nVector      = this->buckets[currentBIdx].n;
				
				if (0.0 < wallWeight)
				{
					if (distFromWall < 0.1)
					{
						distFromWall = 0.1;
					}
					double pressureGrad = this->ps[oneself].pressure / (distFromWall * distFromWall);
					acceleration += nVector * pressureGrad * wallWeight;
				}
			}
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
	g_i = oneself;
	const size_t	currentBX = size_t(this->ps[oneself].center.x / BUCKET_LENGTH);
	const size_t	currentBY = size_t(this->ps[oneself].center.y / BUCKET_LENGTH);
	const size_t	currentBZ = size_t(this->ps[oneself].center.z / BUCKET_LENGTH);

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
		// if (oneself == 2)
		// {
		// 	switch (e)
		// 	{
		// 		case e_VISCOSITY:
		// 			break;
		// 		case e_COLLISION:
		// 			break;
		// 		default:
		// 			break;
		// 	}
		// 	Print::OutWords(this->numOfBuckets, bucketIdx, this->ps[oneself].center, otherBX, otherBY, otherBZ);
		// }
		// if (this->numOfBuckets == bucketIdx)
		// {
		// 	Print::OutWords(this->numOfBuckets, bucketIdx, this->ps[oneself].center, otherBX, otherBY, otherBZ);
		// }
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
									  acceleration, ni);
	this->_SwitchAssignmentOfAcceleration(oneself, e, acceleration, ni);
}

bool	MPS::_CheckOutOfRange(const Vec &pos)
{
	if (pos.x < 0.0 || 
		pos.y < 0.0 || 
		pos.z < 0.0)
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
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		if (this->ps[i].validFlag)
		{
			this->_SearchNeighborParticles(i, e_VISCOSITY, noMeaning);
			this->ps[i].acceleration += this->g;
		}
	}
}

void	MPS::_UpdateVPA1(void)
{
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
	double	minPressure;
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
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		if (this->ps[i].validFlag)
		{
			// this->ps[i].velocity += this->ps[i].acceleration * DELTA_TIME;
			this->ps[i].center += this->ps[i].velocity * DELTA_TIME;

			this->ps[i].validFlag = this->_CheckOutOfRange(this->ps[i].center);
			this->ps[i].acceleration = 0.0;
		}
	}
}

void	MPS::NavierStokesEquations(void)
{
	for (double time = 0.0; time < 0.5; time += 0.5)
	{
		this->BC_UpdateBuckets(this->ps);
		this->_ViscosityAndGravityTerm();
		this->_UpdateVPA1();
		this->_CalcParticlesCollision();
		this->_CalcParticlesPressure();
		this->_PressureGradientTerm();
		this->_UpdateVPA2();
		// this->_CalcParticlesPressure();

		// this->_UpdateVPA3();
		std::cout <<std::fixed << std::setprecision(1)
				  << time / 0.5 * 100
				  << " %\r" << std::flush;
	}
	std::cout <<  std::endl;

	// this->_UpdateBuckets(this->ps);
	// this->_ViscosityAndGravityTerm();
	// this->_UpdateVPA1();
	// this->_CalcParticlesCollision();
	// this->_CalcParticlesPressure();
	// this->_PressureGradientTerm();
	// this->_UpdateVPA2();


	// this->_CalcParticlesPressure();
	// for (size_t i = 0; i < NUM_OF_PARTICLES; ++i)
	// {
	// 	this->pressureLog[i] += this->ps[i].pressure;
	// }
}

// void	MPS::DrawParticles(const Vec &halfMapSize, const double midHeight)
// {
// 	glPointSize(2.0f);
// 	glBegin(GL_POINTS);
// 	glColor3f(0.5, 0.5, 1);
// 	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
// 	{
// 		drawVertex(move_vec_to_map_center(this->ps[i].center, halfMapSize, midHeight));
// 	}
// 	glEnd();
// }

void	MPS::DrawParticles(const Vec &halfMapSize, const double midHeight)
{
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		// if(i == 224 || i == 231)
		// {
		// 	this->ps[i].DrawParticle(halfMapSize, midHeight);

		// }
		this->ps[i].DrawParticle(halfMapSize, midHeight);
	}
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


// double	MPS::W(const size_t i, const size_t oneself, bool gradientFlg) // weight
// {
// 	double	rSQ = this->ps[i].center.Magnitude2d(this->ps[oneself].center);

// 	if (gradientFlg == GRADIENT)
// 	{
// 		return E_RADIUS / rSQ - rSQ / E_RADIUS;
// 	}

// 	return E_RADIUS / rSQ + rSQ / E_RADIUS - 2;
// }