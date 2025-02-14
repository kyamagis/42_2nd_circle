#include "../includes/Side.hpp"

Side::Side()
{

}

Side::Side(Vec S, Vec E):startPoint(S), endPoint(E)
{
	
}

Side::~Side()
{

}

bool	Side::operator==(const Side &side) const
{
	return (this->startPoint == side.startPoint) && 
		   (this->endPoint == side.endPoint);
}

Side&	Side::operator=(const Side &side)
{
	if (this != &side)
	{
		this->startPoint = side.startPoint;
		this->endPoint = side.endPoint;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &ostrm, const Side &side)
{
	return ostrm << '(' << side.startPoint << ", " 
						<< side.endPoint   << ')' 
						<< std::endl;
}