#include "../includes/Particle.hpp"
#include "../includes/Graphic.hpp"
#include "../includes/Utils.hpp"

Particle::Particle():Circle(), pressure(0),  validFlag(true)
{

}

Particle::Particle(const double &cX, 
				   const double &cY, 
				   const double &cZ, 
				   const double &R):Circle(cX, cY, cZ, R), pressure(0), validFlag(true)
{

}

Particle::Particle(const double &cX, 
				   const double &cY, 
				   const double &cZ):Circle(cX, cY, cZ, RADIUS), pressure(0), validFlag(true)
{

}

Particle::Particle(const Particle &p): Circle(p)
{
	*this = p;
}

Particle::~Particle()
{

}

void	Particle::MoveVertexToMapCenterP(const Vec &halfMapSize, const double midHeight)
{
	move_vec_to_map_center(this->center, halfMapSize, midHeight);
}

void	Particle::RotationP(void)
{
	rotation(this->center);
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
		this->pressure = p.pressure;
		this->validFlag = p.validFlag;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &ostrm, const Particle &p)
{
	return ostrm << "pos: " << p.center << ", " 
				 << "vel: "	<< p.velocity << ", " 
				 << "acc: "	<< p.acceleration << ", "
				 << "rad: "	<< p.r 
				 << std::endl;
}
