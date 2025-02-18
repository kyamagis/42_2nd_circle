#include "../includes/ParticleMethod.hpp"

PT::PT():x(0), y(0), r(0)
{

}

PT::PT(const PT &pT)
{
	*this = pT;
}

PT::~PT()
{
	
}

bool	PT::operator==(const PT &pT) const
{
	return (this->x == pT.x) && 
		   (this->y == pT.y) &&
		   (this->r == pT.r);
}

PT&	PT::operator=(const PT &pT)
{
	if (this != &pT)
	{
		this->x = pT.x;
		this->y = pT.y;
		this->r = pT.r;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &ostrm, const PT &pT)
{
	return ostrm << '(' << pT.x << ", " 
						<< pT.y << ", "
						<< pT.r << ')' 
						<< std::endl;
}