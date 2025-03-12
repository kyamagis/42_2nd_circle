#ifndef QUATERNION_HPP
# define QUATERNION_HPP

#include <deque>
#include <iostream>
#include <string>
#include "./Vec.hpp"

class Vec;

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

		bool	operator==(const double &num) const;
		Quaternion operator*(const Quaternion& q) const;
		Quaternion&	operator=(const Quaternion &q);
		void	operator=(const double &num);
};

std::ostream &operator<<(std::ostream &ostrm, const Quaternion &q);

#endif