#include "../includes/Circle.hpp"

Circle::Circle():center(0,0,0) ,r(0)
{

}

Circle::Circle(const double &X, const double &Y, const double &Z, const double &R):center(X,Y,Z) ,r(R)
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
	return (this->center.x == c.center.x) && 
		   (this->center.y == c.center.y) &&
		   (this->center.z == c.center.z) &&
		   (this->r == c.r);
}

Circle&	Circle::operator=(const Circle &c)
{
	if (this != &c)
	{
		this->center.x = c.center.x;
		this->center.y = c.center.y;
		this->center.z = c.center.z;
		this->r = c.r;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &ostrm, const Circle &c)
{
	return ostrm << '(' << c.center.x << ", " 
						<< c.center.y << ", "
						<< c.center.z << ", "
						<< c.r << ')' 
						<< std::endl;
}