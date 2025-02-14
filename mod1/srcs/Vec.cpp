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

bool	Vec::operator==(const Vec &vec) const
{
	return (this->x == vec.x) && 
			(this->y == vec.y) && 
			(this->z == vec.z);
}

Vec::Vec(const Vec &vec)
{
	*this = vec;
}

Vec&	Vec::operator=(const Vec &vec)
{
	if (this != &vec)
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &ostrm, const Vec &Vec)
{
	return ostrm << '(' << Vec.x << ", " 
						<< Vec.y << ", "
						<< Vec.z << ')' 
						<< std::endl;
}