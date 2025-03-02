#ifndef MPS_HPP
# define MPS_HPP

#include <iostream>
#include <string>
#include <deque>
#include "./Vec.hpp"
#include "./Particle.hpp"
#include "./Triangle.hpp"
#include "./BucketsController.hpp"

class MPS: public BC
{
	private:
		double	_n0;
		double	_lambda;

		MPS();
		void	_InitBuckets(const std::deque<Triangle> &ts);
		void	_InitParticlesWaterColumnCollapse(void);
		void	_IniDensityAndLambda(void);
		void	_SearchNeighborParticle(const size_t i);
		void	_SearchNeighborParticles(const size_t oneself);

	public:
		std::deque<Particle> ps;
		const Vec	visibleMapSize;
		const Vec	totalMapSize;

		MPS(const uint32_t mapSize[3], 
		   const std::deque<Triangle>	&ts);
		~MPS();

		double	W(const size_t i, const size_t oneself, bool gradientFlg);
		void	PressureGradientTerm(Vec &p, const size_t oneself);
		void	ViscosityTerm(Vec &vi, const size_t oneself);
		void	NavierStokesEquations(const	Vec &g);
		MPS(const MPS &mps);
		MPS&	operator=(const MPS &mps);
		// bool	operator==(const MPS &mps) const;
};

// std::ostream &operator<<(std::ostream &ostrm, const MPS &mps);

#endif