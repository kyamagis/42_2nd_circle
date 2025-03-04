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
	Vec		position;
} t_bucket;

class BC
{
	private:
		BC();

		const Vec		_visibleMapSize;
		const Vec		_totalMapSize;
		const size_t	bucketRow;
		const size_t	bucketColumn;
		const size_t	bucketDepth;
		const size_t	_columnMultiplDepth;
		t_bucket		*_bucketLast;
		
		size_t	_CalcBucketIdx(size_t bucketX, size_t bucketY, size_t bucketz);
		size_t	_CalcBucketIdx(const Vec &v);
		void	_UpdateParticlesInBuckets(const std::deque<Particle> &ps);
		Vec		_MaxEachCoordinateOfVertex(const Vec &a, 
											const Vec &b,
											const Vec &c);
		Vec		_MinEachCoordinateOfVertex(const Vec &a, 
											const Vec &b,
											const Vec &c);
		double	_CalcShortestDistanceFromVertexSQ(const Triangle &t, 
												const size_t bucketIdx);

		double	_CalcDistanceFromSideSQ(const Vec &a, 
										const Vec &b, 
										const size_t bucketIdx);							  
		double	_CalcShortestDistanceFromSideSQ(const Triangle &t, const size_t bucketIdx);
		
		double  _CalcDistanceFromTriangle(const Triangle &t, const size_t bucketIdx);
		
		double	_CalcShortestDistance(const Triangle &t, const size_t bucketIdx);
		void	_CalcDistanceFromWall(const Triangle &t);

	protected:
		void	_MakeBuckets(const std::deque<Particle> &ps);
		void	_UpdateBuckets(const std::deque<Particle> &ps);

		void	_CalcAllDistanceFromWall(const std::deque<Triangle>	&ts);
		void	_SearchNeighborParticle(const size_t i);
		void	_SearchNeighborParticles(const size_t oneself);

	public:

		const size_t		numOfBuckets;

		t_bucket			*buckets;
		int64_t				*particleNextIdxs;

		
		BC(const Vec &visibleMapSize_,
			const Vec &totalMapSize_);
			virtual ~BC();
			
		void	DrawDisFromWall(void);
		// BC(const BC &BC);
		// BC&	operator=(const BC &BC);
		// bool	operator==(const BC &BC) const;
};

// std::ostream &operator<<(std::ostream &ostrm, const BC &BC);

#endif