#include "../includes/Particle.hpp"

Particle::Particle():Circle()
{

}

Particle::Particle(const double &X, const double &Y, const double &Z, const double &R):Circle(X, Y, Z, R)
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
	}
	return *this;
}

std::ostream &operator<<(std::ostream &ostrm, const Particle &p)
{
	return ostrm << '(' << p.center << ", " 
						<< p.r << ", " 
						<< p.velocity << ')' 
						
						<< std::endl;
}