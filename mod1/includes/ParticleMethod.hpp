#ifndef ParticleMethod_HPP
# define ParticleMethod_HPP

#include <iostream>
#include <string>
#include <deque>
#include "./Vec.hpp"
#include "./Particle.hpp"


class PM
{
	private:

	public:
		std::deque<Particle> ps;

		PM();
		~PM();

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