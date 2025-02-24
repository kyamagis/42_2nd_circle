#ifndef ParticleMethod_HPP
# define ParticleMethod_HPP

#include <iostream>
#include <string>
#include <deque>
#include "./Vec.hpp"
#include "./Particle.hpp"
#include "./Triangle.hpp"

typedef struct s_bucket
{
	size_t	firstPrtIdx; // first particle index
	double	disFromWall;
} t_bucket;

class PM
{
	private:
		size_t	_CalcBucketIdx(size_t bucketX, size_t bucketY, size_t bucketz);
		size_t	_CalcBucketIdx(const Vec &v);
		void	_InitBuckets(void);
		Vec		_MaxEachCoordinateOfVertex(const Vec &a, 
										const Vec &b,
										const Vec &c);
		Vec		_MinEachCoordinateOfVertex(const Vec &a, 
										const Vec &b,
										const Vec &c);
		
		Vec		_CalcBucketCenterPos(const size_t i, const size_t j, const size_t k);
		double	_CalcShortestDistanceFromVertex(const Triangle &t, 
												const Vec &bucketCenterPos);
		double	_CalcShortestDistance(const Triangle &t,
									  const double i, 
									  const double j, 
									  const double k);
		double	_CalcDistanceFromSide(const Vec &a, 
									  const Vec &b, 
									  const Vec &bucketCenterPos);							  
		double	_CalcShortestDistanceFromSide(const Triangle &t, 
											  const Vec &bucketCenterPos);
		double  _CalcDistanceFromTriangle(const Triangle &t, 
												  const Vec &bucketCenterPos);
		void	_CalcDistanceFromWall(const Triangle &t);
		void	_CalcAllDistanceFromWall(const std::deque<Triangle>	&ts);

	public:
		std::deque<Particle> ps;
		const Vec	visibleMapSize;
		Vec			totalMapSize;
	
		size_t		bucketRow;
		size_t		bucketColumn;
		size_t		bucketDepth;
		size_t		numOfBuckets;
		t_bucket	*bucketFirst;
		int64_t		*particleNextIdxs;

		PM();
		PM(const uint32_t mapSize[3], 
		   const int64_t maxHeight, 
		   const std::deque<Triangle>	&ts);
		~PM();

		

		void	SearchNeighborParticles(const size_t oneself);
		double	W(const size_t i, const size_t oneself, bool gradientFlg);
		void	PressureGradientTerm(Vec &p, const size_t oneself);
		void	ViscosityTerm(Vec &vi, const size_t oneself);
		void	NavierStokesEquations(const	Vec &g);
		// PM(const PM &pM);
		// PM&	operator=(const PM &pM);
		// bool	operator==(const PM &pM) const;
};

// std::ostream &operator<<(std::ostream &ostrm, const PM &pM);

#endif