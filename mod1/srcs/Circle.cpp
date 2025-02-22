#include "../includes/Circle.hpp"

Circle::Circle():center(0,0,0) ,r(0)
{

}

Circle::Circle(const double &cX, const double &cY, const double &cZ, const double &R):center(cX,cY,cZ) ,r(R)
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
		this->r = c.r;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &ostrm, const Circle &c)
{
	return ostrm << '(' << c.center << ", " 
						<< c.r << ')' 
						<< std::endl;
}