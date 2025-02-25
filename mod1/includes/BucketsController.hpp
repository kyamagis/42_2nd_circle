#ifndef BucketsController_HPP
# define BucketsController_HPP

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

class BC
{
	private:
		BC();
		
		size_t	_CalcBucketIdx(size_t bucketX, size_t bucketY, size_t bucketz);
		size_t	_CalcBucketIdx(const Vec &v);
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

	protected:
		void	_MakeBuckets(const std::deque<Particle> &ps);
		
		void	_CalcAllDistanceFromWall(const std::deque<Triangle>	&ts);
		void	_SearchNeighborParticle(const size_t i);
		void	_SearchNeighborParticles(const size_t oneself);

	public:
		// std::deque<Particle> ps;
		const Vec	visibleMapSize;
		const Vec	totalMapSize;
	
		const size_t		bucketRow;
		const size_t		bucketColumn;
		const size_t		bucketDepth;
		const size_t		numOfBuckets;
		t_bucket	*bucketFirst;
		int64_t		*particleNextIdxs;

	
		BC(const Vec &visibleMapSize_,
		   const Vec &totalMapSize_);
	    virtual ~BC();

		// BC(const BC &BC);
		// BC&	operator=(const BC &BC);
		// bool	operator==(const BC &BC) const;
};

// std::ostream &operator<<(std::ostream &ostrm, const BC &BC);

#endif