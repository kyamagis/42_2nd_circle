#ifndef VEC_HPP
# define VEC_HPP

#include <iostream>
#include <string>

#include "./Defines.hpp"
#include "./Quaternion.hpp"

class Quaternion;

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
		Vec		Normalized3d() const;


		double	MagnitudeSQ3d(const Vec &p) const;
		double	Magnitude3d(const Vec &p) const;
		double	MagnitudeSQ3d(void) const;
		double	Magnitude3d(void) const;
		double	MagnitudeSQ2d(const Vec &p) const;
		double	MagnitudeSQ2d(void) const;
		double	Magnitude2d(const Vec &p) const;
		double	Magnitude2d(void) const;
		
		Vec		Rotate(const Quaternion &q) const;

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
