#ifndef Particle_HPP
# define Particle_HPP

#include <iostream>
#include <string>
#include "./Circle.hpp"

#define I_DISTANCE 0.1 // interparticle distance
#define E_RADIUS   2.1 * I_DISTANCE // effective radius

class Particle: public Circle
{
	private:

	public:
		Vec	velocity;

		Particle();
		Particle(const double &X, const double &Y, const double &Z, const double &R);
		Particle(const Particle &p);
		~Particle();

		Particle&	operator=(const Particle &p);
		bool		operator==(const Particle &p) const;
};

std::ostream &operator<<(std::ostream &ostrm, const Particle &p);

#endif