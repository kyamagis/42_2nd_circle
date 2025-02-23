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
	size_t	i;
	double	disFromWall;
} t_bucket;


class PM
{
	private:

	public:
		std::deque<Particle> ps;
		const Vec	visibleMapSize;
		Vec			totalMapSize;
	
		size_t		bucketRow;
		size_t		bucketColumn;
		size_t		bucketDepth;
		size_t		numOfBuckets;
		t_bucket	*bucketFirst;
		int64_t		*bucketNextIdxs;

		PM();
		PM(const uint32_t mapSize[2], 
		   const int64_t maxHeight, 
		   const std::deque<Triangle>	&ts);
		~PM();

		void	InitBuckets(void);
		void	CalcDistanceFromWall(const Triangle	&t);
		void	CalcAllDistanceFromWall(const std::deque<Triangle>	&ts);
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