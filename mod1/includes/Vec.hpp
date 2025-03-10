#ifndef VEC_HPP
# define VEC_HPP

#include <iostream>
#include <string>

#include "./Defines.hpp"

class Vec
{
	private:

	public:
		double x;
		double y;
		double z;

		Vec();
		Vec(const double cX, const double cY, const double cZ);
		Vec(const double num);
		Vec(const Vec &vec);
		~Vec();

		double	Interpolate2d(const Vec &p, const double dis) const;

		double	DotProduct2d(const Vec &p) const;
		double	DotProduct3d(const Vec &p) const;

		Vec		CrossProduct3d(const Vec &p) const;

		double	MagnitudeSQ3d(const Vec &p) const;
		double	Magnitude3d(const Vec &p) const;
		double	MagnitudeSQ3d(void) const;
		double	Magnitude3d(void) const;
		double	MagnitudeSQ2d(const Vec &p) const;
		double	MagnitudeSQ2d(void) const;
		double	Magnitude2d(const Vec &p) const;
		double	Magnitude2d(void) const;
		
		void	RotationZ(const double rad);
		void	RotationX(const double rad);
		void	RotationY(const double rad);
		Vec		Rotate(const Vec &rad) const;
		Vec		Rotate(const Vec &rad, const unsigned key) const;

		bool	operator==(const Vec &vec) const;
		Vec&	operator=(const Vec &vec);
		void	operator=(const double num);
		Vec		operator+(const Vec &vec) const;
		void	operator+=(const Vec &vec);
		Vec		operator-(const Vec &vec) const;
		void	operator-=(const double n);
		void	operator-=(const Vec &vec);
		Vec		operator*(const Vec &vec) const;
		Vec		operator*(const double a) const;
		void	operator*=(const double a);
		Vec		operator/(const Vec &vec) const;
		Vec		operator/(const double a) const;
		void	operator/=(const Vec &vec);
		void	operator/=(const double num);
};

std::ostream &operator<<(std::ostream &ostrm, const Vec &Vec);

#endif