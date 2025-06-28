#include "../includes/Particle.hpp"
#include "../includes/Utils.hpp"

Particle::Particle():Circle(), velocity(), acceleration(), pressure(0),  validFlag(true)
{

}

Particle::Particle(const double &cX, 
				   const double &cY, 
				   const double &cZ, 
				   const double &R):Circle(cX, cY, cZ, R, R * R), velocity(), acceleration(), pressure(0), validFlag(true)
{

}

Particle::Particle(const double &cX, 
				   const double &cY, 
				   const double &cZ):Circle(cX, cY, cZ, RADIUS, RADIUS * RADIUS), 
				   					 velocity(), acceleration(), pressure(0), validFlag(true)
{

}

Particle::Particle(const Particle &p): Circle(p)
{
	*this = p;
}

Particle::~Particle()
{

}

bool	Particle::operator==(const Particle &p) const
{
	return (this->center == p.center) && 
		   (this->r == p.r) &&
		   (this->velocity == p.velocity);
}

Particle&	Particle::operator=(const Particle &p)
{
	if (this != &p)
	{
		this->center = p.center;
		this->r = p.r;
		this->velocity = p.velocity;
		this->acceleration = p.acceleration;
		this->n = p.n;
		this->pressure = p.pressure;
		this->validFlag = p.validFlag;
		this->bucketX = p.bucketX;
		this->bucketY = p.bucketY;
		this->bucketZ = p.bucketZ;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &ostrm, const Particle &p)
{
	return ostrm << "pos: " << p.center << ", " 
				 << "vel: "	<< p.velocity << ", " 
				 << "acc: "	<< p.acceleration << ", "
				 << "prs: "	<< p.pressure << ", "
				 << "rad: "	<< p.r ;
}
