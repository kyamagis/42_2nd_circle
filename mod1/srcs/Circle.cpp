#include <cmath>
#include <omp.h>

#include "../includes/Circle.hpp"
#include "../includes/Utils.hpp"

Circle::Circle():center(0,0,0) ,r(0), rSQ(0)
{

}

Circle::Circle(const double &cX, 
			   const double &cY, 
			   const double &cZ, 
			   const double &_r,
			   const double &_rSQ):center(cX,cY,cZ) ,r(_r), rSQ(_rSQ)
{

}

Circle::Circle(const Circle &c)
{
	*this = c;
}

Circle::~Circle()
{

}

bool	Circle::operator==(const Circle &c) const
{
	return (this->center == c.center) &&
		   (this->r == c.r);
}

Circle&	Circle::operator=(const Circle &c)
{
	if (this != &c)
	{
		this->center = c.center;
		this->color = c.color;
		this->r = c.r;
		this->rSQ = c.rSQ;
	}
	return *this;
}

std::ostream	&operator<<(std::ostream &ostrm, const Circle &c)
{
	return ostrm << '(' << c.center << ", " 
						<< c.r << ')' ;
}
