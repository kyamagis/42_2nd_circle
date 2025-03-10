#ifndef Quaternion_HPP
# define Quaternion_HPP

#include <deque>
#include <iostream>
#include <string>
#include "./Vec.hpp"
#include "./Circle.hpp"

class Quaternion
{
	private:
	
	public:
		double	w;
		double	x;
		double	y;
		double	z;

		Quaternion();
		~Quaternion();
		Quaternion(double _w, double _x, double _y, double _z);
		Quaternion(const double _w, const Vec &pos);
		Quaternion(const Quaternion& q);

		Quaternion conjugate() const;

		Quaternion operator*(const Quaternion& q) const;
		Quaternion&	operator=(const Quaternion &q);
};

std::ostream &operator<<(std::ostream &ostrm, const Quaternion &q);

#endif