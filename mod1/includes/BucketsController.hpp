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
	double	distFromWall;
	double	distFromWalls[8];
	Vec		position;
	Vec		center;
	Vec		n;
	Vec		nInterpolation;
	Vec		shortestVec;
	size_t	bucketX;
	size_t	bucketY;
	size_t	bucketZ;
} t_bucket;

class BC
{
	private:
		BC();

		const Vec				_visibleMapSize;
		const Vec				_totalMapSize;
		const std::deque<Vec>	_weights;
		t_bucket				*_bucketLast;
	
		void	_UpdateParticlesInBuckets(const std::deque<Particle> &ps);
		Vec		_MaxEachCoordinateOfVertex(const Vec &a, 
											const Vec &b,
											const Vec &c);
		Vec		_MinEachCoordinateOfVertex(const Vec &a, 
											const Vec &b,
											const Vec &c);

		Vec		_CalcShortestVecFromVertex(const Triangle &t, const size_t bucketIdx);
		Vec		_CalcVecFromSide(const Vec &a, const Vec &b, const size_t bucketIdx);
		Vec		_CalcShortestVecFromSide(const Triangle &t, const size_t bucketIdx);
		Vec		_CalcVecFromTriangle(const Triangle &t, const size_t bucketIdx);
		Vec		_CalcShortestVec(const Triangle &t, const size_t bucketIdx);


		double	_CalcShortestDistanceFromVertexSQ(const Triangle &t, 
												const size_t bucketIdx);
		void	_CalcBucketsPos(const size_t i);
		double	_CalcDistanceFromSideSQ(const Vec &a, 
										const Vec &b, 
										const size_t bucketIdx);							  
		double	_CalcShortestDistanceFromSideSQ(const Triangle &t, const size_t bucketIdx);
		double  _CalcDistanceFromTriangleSQ(const Triangle &t, const size_t bucketIdx);
		double	_CalcShortestDistanceSQ(const Triangle &t, const size_t bucketIdx);
		void	_CalcDistanceFromWallSQ(const Triangle &t);

		double	_GetDistFromWall(const size_t currentBX,
									const size_t currentBY,
									const size_t currentBZ);
		void	_GetNeighborBDistFromWalls(const size_t bucketIdx, double *distFromWalls);

		Vec		_GetNVEC(const size_t currentBX,
						 	   const size_t currentBY,
						 	   const size_t currentBZ);
		
		double	_SearchNeighborBDistFromWall(size_t currentBX,
											size_t currentBY,
											size_t currentBZ,
											const unsigned char cmp);
		
		bool	_StoreEachCmpOfNeighborBDistFromWall(const size_t currentBX, 
													const size_t currentBY, 
													const size_t currentBZ,
													const unsigned char cmp,
													double &neighborBDistFromWall);
		
	protected:
		const double	bc_radius_effective;
		const double	bc_radius_effectiveSQ;

		void	BC_MakeBuckets(const std::deque<Particle> &ps);
		void	BC_CalcAllDistanceFromWallSQ(const std::deque<Triangle>	&ts);
		size_t	BC_CalcBucketIdx(size_t bucketX, size_t bucketY, size_t bucketz);
		size_t	BC_CalcBucketIdx(const Vec &v);
		void	BC_UpdateBuckets(const std::deque<Particle> &ps);

		bool	BC_IsOutOfWallWeightRange(const double disFromWall);
		double	BC_InterpolateWallWeight(const double interpolatedDist);
										
	public:

		const double	bc_bucketLength;
		const size_t	bucketRow;
		const size_t	bucketColumn;
		const size_t	bucketDepth;
		const size_t	_columnMultiplDepth;
		const size_t	numOfBuckets;


		t_bucket			*buckets;
		size_t				*particleNextIdxs;

		BC(const Vec &visibleMapSize_,
			const Vec &totalMapSize_);
			virtual ~BC();

		// BC(const BC &BC);
		// BC&	operator=(const BC &BC);
		// bool	operator==(const BC &BC) const;
};

// std::ostream &operator<<(std::ostream &ostrm, const BC &BC);

#endif