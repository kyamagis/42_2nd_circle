#include "../includes/Quaternion.hpp"
#include <cmath>

Quaternion::Quaternion(): w(0), x(0), y(0), z(0)
{

}

Quaternion::Quaternion(const double _w, const double _x, const double _y, const double _z): w(_w), x(_x), y(_y), z(_z)
{

}

Quaternion::Quaternion(const double _w, const Vec &pos): w(_w), x(pos.x), y(pos.y), z(pos.z)
{

}

Quaternion::Quaternion(const Quaternion& q)
{
	*this = q;
}


Quaternion::~Quaternion()
{

}

Quaternion	Quaternion::conjugate() const
{
	return	Quaternion(this->w, - this->x, - this->y, - this->z);
}

Quaternion	Quaternion::calcQuaternion(const double radian, const Vec &direction)
{
	Quaternion q = make_rotate_quaternion(radian, direction);
	if (*this == 0.0)
	{
		return q;
	}
	return q * (*this);
}

bool	Quaternion::operator==(const double &num) const
{
	return (this->w == num) && 
		   (this->x == num) &&
		   (this->y == num) &&
		   (this->z == num);
}

void	Quaternion::operator=(const double &num)
{
	this->w = num;
	this->x = num;
	this->y = num;
	this->z = num;
}

Quaternion&	Quaternion::operator=(const Quaternion &q)
{
	if (this != &q)
	{
		this->w = q.w;
		this->x = q.x;
		this->y = q.y;
		this->z = q.z;
	}
	return *this;
}

Quaternion Quaternion::operator*(const Quaternion& q) const {
	return Quaternion
	(
		this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z,
		this->w * q.x + this->x * q.w + this->y * q.z - this->z * q.y,
		this->w * q.y - this->x * q.z + this->y * q.w + this->z * q.x,
		this->w * q.z + this->x * q.y - this->y * q.x + this->z * q.w
	);
}

std::ostream &operator<<(std::ostream &ostrm, const Quaternion &q)
{
	return ostrm << '(' << q.w << ", " 
						<< q.x << ", "
						<< q.y << ", "
						<< q.z << ')' 
						<< std::endl;
}


Quaternion	make_rotate_quaternion(const double radian, const Vec &direction)
{
	return Quaternion(cos(radian / 2.0), direction * sin(radian / 2.0));
}
