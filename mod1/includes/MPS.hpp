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
		double	_cffVTerm;
		double	_cffPress;
		double	_cffPGTerm;

		MPS();
		void	_InitBuckets(const std::deque<Triangle> &ts);
		void	_InitParticlesWaterColumnCollapse(void);

		void	_InitTermCoefficient(void);
		void	_SetParameter(void);
		void	_CalcEachViscosity(const size_t oneself);
		void	_UpdateVPA(void);
		void	_ViscosityAndGravityTerm(void);
		void	_CalcCollision(void);

	public:
		std::deque<Particle> ps;
		const Vec	visibleMapSize;
		const Vec	totalMapSize;
		Vec			g;

		MPS(const uint32_t mapSize[3], 
		   const std::deque<Triangle> &ts);
		~MPS();
		MPS(const MPS &mps);
		MPS&	operator=(const MPS &mps);

		double	W(const size_t i, const size_t oneself, bool gradientFlg);
		void	PressureGradientTerm(Vec &p, const size_t oneself);
		void	NavierStokesEquations(void);
		void	MoveVertexToMapCenterPs(const Vec &halfMapSize, const double midHeight);
		void	RotationPs(void);
		void	DrawParticles(void);

		// bool	operator==(const MPS &mps) const;
};

// std::ostream &operator<<(std::ostream &ostrm, const MPS &mps);

#endif