#include <cmath>
#include "../includes/Vec.hpp"

Vec::Vec():x(0), y(0), z(0)
{

}

Vec::Vec(double X, double Y, double Z):x(X), y(Y), z(Z)
{
	
}

Vec::Vec(const Vec &vec)
{
	*this = vec;
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

double	Vec::DotProduct2d(const Vec &p) const
{
	return (this->x * p.x) + (this->y * p.y);
}

double	Vec::DotProduct3d(const Vec &p) const
{
	return (this->x * p.x) + (this->y * p.y) + (this->z * p.z);
}

double	Vec::MagnitudeSQ2d(const Vec &p) const
{
	return (this->x - p.x) * (this->x - p.x) + 
		   (this->y - p.y) * (this->y - p.y);
}

double	Vec::MagnitudeSQ2d(void) const
{
	return this->x * this->x + this->y * this->y;
}

double	Vec::Magnitude2d(const Vec &p) const
{
	return sqrt(MagnitudeSQ2d(p));
}

double	Vec::Magnitude2d(void) const
{
	return sqrt(MagnitudeSQ2d());
}

void	Vec::RotationZ(double rad)
{
	double	tempX = this->x;
	double	tempY = this->y;

	this->x = tempX * cos(rad) - tempY * sin(rad);
	this->y = tempX * sin(rad) + tempY * cos(rad);
}

void	Vec::RotationX(double rad)
{
	double	tempY = this->y;
	double	tempZ = this->z;

	this->y =  tempY * cos(rad) - tempZ * sin(rad);
	this->z =  tempY * sin(rad) + tempZ * cos(rad);
}

void	Vec::RotationY(double rad)
{
	double	tempX = this->x;
	double	tempZ = this->z;

	this->x =  tempX * cos(rad) + tempZ * sin(rad);
	this->z = -tempX * sin(rad) + tempZ * cos(rad);
}

// double	Vec::cos_angle(const Vec &a, const Vec &b)
// {
// 	return dot_product_2d(a, b) 
// 			/ (Magnitude2d(a) * Magnitude2d(b));
// }


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
