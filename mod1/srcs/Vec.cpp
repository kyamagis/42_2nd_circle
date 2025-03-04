#include <cmath>
#include "../includes/Vec.hpp"

Vec::Vec():x(0), y(0), z(0)
{

}

Vec::Vec(double cX, double cY, double cZ):x(cX), y(cY), z(cZ)
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

bool	Vec::NearlyEqual(const Vec &vec) const
{
	// double	leverage = 100000;

	// return (size_t(this->x * leverage) == size_t(vec.x * leverage)) && 
	// 	   (size_t(this->y * leverage) == size_t(vec.y * leverage)) && 
	// 	   (size_t(this->z * leverage) == size_t(vec.z * leverage));

	// if (this->x * vec.x < 0.0)
	// {
	// 	return false ;
	// }
	// if (this->y * vec.y < 0.0)
	// {
	// 	return false ;
	// }
	// if (this->z * vec.z < 0.0)
	// {
	// 	return false ;
	// }
	// return true;

	// if (std::signbit(this->x) != std::signbit(vec.x))
	// {
	// 	return false ;
	// }
	// if (std::signbit(this->y) != std::signbit(vec.y))
	// {
	// 	return false ;
	// }
	// if (std::signbit(this->z) != std::signbit(vec.z))
	// {
	// 	return false ;
	// }
	if (std::signbit(this->x) != std::signbit(vec.x))
	{
		return false ;
	}
	if (std::signbit(this->y) != std::signbit(vec.y))
	{
		return false ;
	}
	if (std::signbit(this->z) != std::signbit(vec.z))
	{
		return false ;
	}
	return true;
}

double	Vec::DotProduct2d(const Vec &p) const
{
	return (this->x * p.x) + (this->y * p.y);
}

double	Vec::DotProduct3d(const Vec &p) const
{
	return (this->x * p.x) + (this->y * p.y) + (this->z * p.z);
}

Vec	Vec::CrossProduct3d(const Vec &p) const
{
	const double	crossX = this->y * p.z - this->z * p.y;
	const double	crossY = this->z * p.x - this->x * p.z;
	const double	crossZ = this->x * p.y - this->y * p.x;

	return Vec(crossX, crossY, crossZ);
}

double	Vec::MagnitudeSQ3d(const Vec &p) const
{
	return (this->x - p.x) * (this->x - p.x) + 
		   (this->y - p.y) * (this->y - p.y) +
		   (this->z - p.z) * (this->z - p.z);
}

double	Vec::Magnitude3d(const Vec &p) const
{
	return sqrt(this->MagnitudeSQ3d(p));
}

double	Vec::MagnitudeSQ3d(void) const
{
	return this->x * this->x  + 
		   this->y * this->y +
		   this->z * this->z;
}

double	Vec::Magnitude3d(void) const
{
	return sqrt(this->MagnitudeSQ3d());
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
	return sqrt(this->MagnitudeSQ2d(p));
}

double	Vec::Magnitude2d(void) const
{
	return sqrt(this->MagnitudeSQ2d());
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

void	Vec::operator=(const double num)
{
	this->x = num;
	this->y = num;
	this->z = num;
}

Vec	Vec::operator+(const Vec &vec) const
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

void	Vec::operator-=(const double n)
{
	this->x -= n;
	this->y -= n;
	this->z -= n;
}

void	Vec::operator-=(const Vec &vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	this->z -= vec.z;
}

Vec	Vec::operator*(const Vec &vec) const
{
	return Vec(this->x * vec.x, this->y * vec.y, this->z * vec.z);
}

Vec	Vec::operator*(const double a) const
{
	return Vec(this->x * a, this->y * a, this->z * a);
}

void	Vec::operator*=(const double a)
{
	this->x *= a;
	this->y *= a;
	this->z *= a;
}

Vec	Vec::operator/(const Vec &vec) const
{
	return Vec(this->x / vec.x, this->y / vec.y, this->z / vec.z);
}

Vec	Vec::operator/(const double a) const
{
	return Vec(this->x / a, this->y / a, this->z / a);
}

void	Vec::operator/=(const Vec &vec)
{
	this->x /= vec.x;
	this->y /= vec.y;
	this->z /= vec.z;
}

void	Vec::operator/=(const double num)
{
	this->x /= num;
	this->y /= num;
	this->z /= num;
}

std::ostream &operator<<(std::ostream &ostrm, const Vec &Vec)
{
	return ostrm << '(' << Vec.x << ", " 
						<< Vec.y << ", "
						<< Vec.z << ')';
}
