#include "../includes/Vec.hpp"

Vec::Vec()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vec::Vec(int32_t X, int32_t Y, int32_t Z):x(X), y(Y), z(Z)
{
	
}

Vec::~Vec()
{

}


std::ostream &operator<<(std::ostream &ostrm, const Vec &Vec)
{
	return ostrm << '(' << Vec.x << ", " 
						<< Vec.y << ", "
						<< Vec.z << ')' 
						<< std::endl;
}