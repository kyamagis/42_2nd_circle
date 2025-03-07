#include "../includes/Quaternion.hpp"

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

Quaternion Quaternion::conjugate() const
{
	return	Quaternion(this->w, - this->x, - this->y, - this->z);
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
	return Quaternion(
		w * q.w - x * q.x - y * q.y - z * q.z,
		w * q.x + x * q.w + y * q.z - z * q.y,
		w * q.y - x * q.z + y * q.w + z * q.x,
		w * q.z + x * q.y - y * q.x + z * q.w
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