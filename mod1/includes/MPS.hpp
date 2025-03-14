#ifndef MPS_HPP
# define MPS_HPP

#include <iostream>
#include <string>
#include <deque>
#include "./Vec.hpp"
#include "./Particle.hpp"
#include "./Triangle.hpp"
#include "./BucketsController.hpp"

enum	e_operation
{
	e_VISCOSITY,
	e_COLLISION,
	e_PRESSURE,
	e_PGRADIENT1,
	e_PGRADIENT2,
};

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

		bool	_CheckOutOfRange(const Vec &pos);
		
		double	_WallWeight(const double disFromWall);
		void	_CalcOneOnOneViscosity(const Vec &oneselfVel, 
										Vec &acceleration, 
										const size_t particleIdx,
										const double distanceSQP);
		void	_CalcOneOnOneCollision(const Vec &oneselfVel,
										Vec &acceleration, 
										const size_t particleIdx,
										const Vec &dr,
										const double distanceSQP);
		void	_CalcOneOnOnePressure(const double distanceSQP, double &ni);
		void	_SmallerPressure(double &minPressure, const size_t particleIdx, const double distanceSQP);
		void	_CalcOneOnOnePressureGradient(double &minPressure,
												Vec &acceleration, 
												const size_t particleIdx,
												const Vec &dr,
												const double distanceSQP);
		void	_SwitchOperation(const e_operation e, 
								double &minPressure,
								const Vec &oneselfPos,
								const Vec &oneselfVel, 
								Vec &acceleration, 
								const size_t particleIdx,
								double &ni);

		void	_SwitchContributionFromWall(const size_t oneself, const e_operation e, 
											const size_t currentBX,
											const size_t currentBY,
											const size_t currentBZ, 
											Vec &acceleration, double &ni);
		void	_SwitchAssignmentOfAcceleration(const size_t oneself, const e_operation e, 
												const Vec &acceleration, const double ni);

		void	_SearchNeighborParticles(const size_t oneself, const e_operation e, double &minPressure);
		void	_ViscosityAndGravityTerm(void);
		void	_CalcParticlesCollision(void);
		void	_CalcParticlesPressure(void);
		void	_PressureGradientTerm(void);
		void	_UpdateVPA1(void);
		void	_UpdateVPA2(void);
		void	_UpdateVPA3(void);

	public:
		std::deque<Particle> ps;
		const Vec	visibleMapSize;
		const Vec	totalMapSize;
		Vec			g;
		double		n0;

		MPS(const Vec mapSize, 
		   const std::deque<Triangle> &ts);
		~MPS();
		MPS(const MPS &mps);
		MPS&	operator=(const MPS &mps);

		double	W(const size_t i, const size_t oneself, bool gradientFlg);
		void	NavierStokesEquations(void);
		void	DrawParticles(const Vec &halfMapSize, const double midHeight);

		// bool	operator==(const MPS &mps) const;
};

// std::ostream &operator<<(std::ostream &ostrm, const MPS &mps);

#endif
