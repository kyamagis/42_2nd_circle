#include <cmath>
#include "../includes/Quaternion.hpp"
#include "../includes/Vec.hpp"

Vec::Vec():x(0), y(0), z(0)
{

}

Vec::Vec(const double cX, const double cY, const double cZ):x(cX), y(cY), z(cZ)
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

double	Vec::Interpolate2d(const Vec &p, const double dis) const
{
	const double	slope = (this->y - p.y) / (this->x - p.x);
	const double	intercept = this->y - slope * this->x;

	return slope * dis + intercept;
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

void	Vec::RotationZ(const double rad)
{
	double	tempX = this->x;
	double	tempY = this->y;

	this->x = tempX * cos(rad) - tempY * sin(rad);
	this->y = tempX * sin(rad) + tempY * cos(rad);
}

void	Vec::RotationX(const double rad)
{
	double	tempY = this->y;
	double	tempZ = this->z;

	this->y =  tempY * cos(rad) - tempZ * sin(rad);
	this->z =  tempY * sin(rad) + tempZ * cos(rad);
}

void	Vec::RotationY(const double rad)
{
	double	tempX = this->x;
	double	tempZ = this->z;

	this->x =  tempX * cos(rad) + tempZ * sin(rad);
	this->z = -tempX * sin(rad) + tempZ * cos(rad);
}

Vec	Vec::Rotate(const Vec &rad) const
{
	Quaternion qx(cos(rad.x / 2.0), Vec(1,0,0) * sin(rad.x / 2.0));
	Quaternion qy(cos(rad.y / 2.0), Vec(0,1,0) * sin(rad.y / 2.0));
	Quaternion qz(cos(rad.z / 2.0), Vec(0,0,1) * sin(rad.z / 2.0));

	Quaternion q = qz * qy * qx;

	Quaternion pos(0.0, *this);
	Quaternion rotated = q * pos * q.conjugate();

	return Vec(rotated.x, rotated.y, rotated.z);
}

Vec	Vec::Rotate(const Vec &rad, const unsigned key) const
{
	Quaternion qx(cos(rad.x / 2.0), Vec(1,0,0) * sin(rad.x / 2.0));
	Quaternion qy(cos(rad.y / 2.0), Vec(0,1,0) * sin(rad.y / 2.0));
	Quaternion qz(cos(rad.z / 2.0), Vec(0,0,1) * sin(rad.z / 2.0));

	Quaternion q;

	switch (key)
	{
		case 'x':
		case 'X':
			q = qx * qy * qz;
			break;
		case 'y':
		case 'Y':
			q = qy * qz * qx;
			break;
		case 'z':
		case 'Z':
			q = qz * qx * qy;
			break;
		default:
			q = qz * qx * qy;
			break;
	}

	Quaternion pos(0.0, *this);
	Quaternion rotated = q * pos * q.conjugate();

	return Vec(rotated.x, rotated.y, rotated.z);
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
