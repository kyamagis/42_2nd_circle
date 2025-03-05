#ifndef Particle_HPP
# define Particle_HPP

#include <iostream>
#include <string>

#include "./Defines.hpp"
#include "./Circle.hpp"

class Particle: public Circle
{
	private:

	public:
		Vec	velocity;
		Vec	acceleration;

		Particle();
		Particle(const double &cX, const double &cY, const double &cZ);
		Particle(const double &cX, const double &cY, const double &cZ, const double &R);
		Particle(const Particle &p);
		~Particle();

		Particle&	operator=(const Particle &p);
		bool		operator==(const Particle &p) const;
};

std::ostream &operator<<(std::ostream &ostrm, const Particle &p);

#endif