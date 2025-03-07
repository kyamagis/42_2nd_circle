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
	if (coor < max + 1)
	{
		return  coor + 1;
	}
	return coor;
}

void	MPS::_CalcEachViscosity(const size_t oneself)
{
	if (this->ps[oneself].center.x < 0.0 ||
		this->ps[oneself].center.y < 0.0 ||
		this->ps[oneself].center.z < 0.0)
	{
		return ;
	}

	const size_t	currentBX = size_t(this->ps[oneself].center.x / BUCKET_LENGTH);
	const size_t	currentBY = size_t(this->ps[oneself].center.y / BUCKET_LENGTH);
	const size_t	currentBZ = size_t(this->ps[oneself].center.z / BUCKET_LENGTH);

	const Vec	oneselfPos(this->ps[oneself].center.x, this->ps[oneself].center.y, this->ps[oneself].center.z);
	const Vec	oneselfV(this->ps[oneself].velocity.x, this->ps[oneself].velocity.y, this->ps[oneself].velocity.z);
	const Vec	oneselfA(this->ps[oneself].acceleration.x, this->ps[oneself].acceleration.y, this->ps[oneself].acceleration.z);
	Vec	acceleration;

	const size_t	otherBEdgeX = _InitOtherBucketCoor(currentBX);
	const size_t	otherBEdgeY = _InitOtherBucketCoor(currentBY);
	const size_t	otherBEdgeZ = _InitOtherBucketCoor(currentBZ);

	if (oneself == 0)
	{
		std::cout << currentBX << " " << currentBY << " " << currentBZ << std::endl;
	}

	const size_t	maxBX = _InitMaxOtherBucketCoor(this->bucketRow,    currentBX);
	const size_t	maxBY = _InitMaxOtherBucketCoor(this->bucketColumn, currentBY);
	const size_t	maxBZ = _InitMaxOtherBucketCoor(this->bucketDepth,  currentBZ);

	size_t	bucketIdx;
	size_t	particleIdx;

	Vec		dr;
	double	distanceSQP;
	double	distanceP;
	double	wallWeight;
	double	w;

	if (oneself == 0)
	{
		std::cout << "_ViscosityAndGravityTerm" << std::endl;
	}

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
			dr.x = this->ps[particleIdx].center.x - oneselfPos.x;
			dr.y = this->ps[particleIdx].center.y - oneselfPos.y;
			dr.z = this->ps[particleIdx].center.z - oneselfPos.z;
			distanceSQP = dr.MagnitudeSQ3d();
			if (particleIdx != oneself) 
			{
				if (distanceSQP < E_RADIUS_SQ) 
				{
					if (distanceSQP <= 0.0)
					{
						std::cout << distanceP << std::endl;
					}
					distanceP = sqrt(distanceSQP);
					w = WEIGHT(distanceP);
					acceleration.x += (this->ps[particleIdx].velocity.x - oneselfV.x) * w;
					acceleration.y += (this->ps[particleIdx].velocity.y - oneselfV.y) * w;
					acceleration.z += (this->ps[particleIdx].velocity.z - oneselfV.z) * w;
				}
				wallWeight  = this->buckets[bucketIdx].wallWeight;
				if (0.0 < wallWeight)
				{
					acceleration.x += (- 2.0 * oneselfV.x) * wallWeight;
					acceleration.y += (- 2.0 * oneselfV.y) * wallWeight;
					acceleration.z += (- 2.0 * oneselfV.z) * wallWeight;
				}
			}
			particleIdx = this->particleNextIdxs[particleIdx];
			if (particleIdx == UINT64_MAX)
			{
				break;
			}
		}
	}}}
	this->ps[oneself].acceleration = acceleration * this->_cffVTerm;

	if (oneself == 0)
	{
		std::cout << "end" << std::endl;
	}
}

void	MPS::_UpdateVPA(void)
{
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		this->ps[i].velocity.x += this->ps[i].acceleration.x * DELTA_TIME;
		this->ps[i].velocity.y += this->ps[i].acceleration.y * DELTA_TIME;
		this->ps[i].velocity.z += this->ps[i].acceleration.z * DELTA_TIME;

		this->ps[i].center.x += this->ps[i].velocity.x * DELTA_TIME;
		this->ps[i].center.y += this->ps[i].velocity.y * DELTA_TIME;
		this->ps[i].center.z += this->ps[i].velocity.z * DELTA_TIME;

		this->ps[i].acceleration.x = 0.0;
		this->ps[i].acceleration.y = 0.0;
		this->ps[i].acceleration.z = 0.0;
	}
	// std::cout << this->ps[0] << std::endl;
}

void	MPS::_ViscosityAndGravityTerm(void)
{
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		this->_CalcEachViscosity(i);
		this->ps[i].acceleration += this->g;
		
	}
	
	this->_UpdateVPA();
}

void	MPS::_CalcCollision(void)
{
	
}

void	MPS::NavierStokesEquations(void)
{
	this->_ViscosityAndGravityTerm();
	this->_UpdateBuckets(this->ps);
}

void	MPS::MoveVertexToMapCenterPs(const Vec &halfMapSize, const double midHeight)
{
	for (size_t	i = 0; i < this->ps.size(); ++i)
	{
		this->ps[i].MoveVertexToMapCenterP(halfMapSize, midHeight);
	}
}

void	MPS::RotationPs(void)
{
	for (size_t	i = 0; i < this->ps.size(); ++i)
	{
		this->ps[i].RotationP();
	}
	rotation(this->g);
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
