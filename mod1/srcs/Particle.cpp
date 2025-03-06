#include "../includes/Particle.hpp"
#include "../includes/Graphic.hpp"
#include "../includes/Utils.hpp"

Particle::Particle():Circle()
{

}

Particle::Particle(const double &cX, 
				   const double &cY, 
				   const double &cZ, 
				   const double &R):Circle(cX, cY, cZ, R)
{

}

Particle::Particle(const double &cX, 
				   const double &cY, 
				   const double &cZ):Circle(cX, cY, cZ, RADIUS)
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
	}
	return *this;
}

std::ostream &operator<<(std::ostream &ostrm, const Particle &p)
{
	return ostrm << '(' << p.center << ", " 
						<< p.velocity << ", " 
						<< p.acceleration << ", "
						<< p.r << ')'  
						
						<< std::endl;
}
