#include "../includes/Vec.hpp"

Vec::Vec():x(0), y(0), z(0)
{

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

Vec	Vec::operator+(const Vec &vec)
{
	return Vec(this->x + vec.x, this->y + vec.y, this->z + vec.z);
}

void	Vec::operator+=(const Vec &vec)
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;
}

Vec	Vec::operator-(const Vec &vec) const
{
	return Vec(this->x - vec.x, this->y - vec.y, this->z - vec.z);
}

Vec	Vec::operator*(const Vec &vec)
{
	return Vec(this->x * vec.x, this->y * vec.y, this->z * vec.z);
}

Vec	Vec::operator*(const double a)
{
	return Vec(this->x * a, this->y * a, this->z * a);
}

void	Vec::operator*=(const double a)
{
	this->x *= a;
	this->y *= a;
	this->z *= a;
}


Vec	Vec::operator/(const Vec &vec)
{
	return Vec(this->x / vec.x, this->y / vec.y, this->z / vec.z);
}

std::ostream &operator<<(std::ostream &ostrm, const Vec &Vec)
{
	return ostrm << '(' << Vec.x << ", " 
						<< Vec.y << ", "
						<< Vec.z << ')';
}