#ifndef Particle_HPP
# define Particle_HPP

#include <iostream>
#include <string>

#include "./Defines.hpp"
#include "./Circle.hpp"

class Vec;

class Particle: public Circle
{
	private:

	public:
		Vec	velocity;
		Vec	acceleration;
		Vec	n;
		double	pressure;
		bool	validFlag;
		size_t	bucketX;
		size_t	bucketY;
		size_t	bucketZ;

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