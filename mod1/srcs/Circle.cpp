#include "../includes/Circle.hpp"

Circle::Circle():x(0), y(0), r(0)
{

}

Circle::Circle(const Circle &circle)
{
	*this = circle;
}

Circle::~Circle()
{

}

bool	Circle::operator==(const Circle &circle) const
{
	return (this->x == circle.x) && 
		   (this->y == circle.y) &&
		   (this->r == circle.r);
}

Circle&	Circle::operator=(const Circle &circle)
{
	if (this != &circle)
	{
		this->x = circle.x;
		this->y = circle.y;
		this->r = circle.r;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &ostrm, const Circle &circle)
{
	return ostrm << '(' << circle.x << ", " 
						<< circle.y << ", "
						<< circle.r << ')' 
						<< std::endl;
}