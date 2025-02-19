#ifndef VEC_HPP
# define VEC_HPP

#include <iostream>
#include <string>

#define	MAX_COORDINATE 1000000
#define	MIN_COORDINATE 0

class Vec
{
	private:

	public:
		double x;
		double y;
		double z;

		Vec();
		Vec(double X, double Y, double Z);
		Vec(const Vec &vec);
		~Vec();

		double	DotProduct2d(const Vec &p) const;
		double	DotProduct3d(const Vec &p) const;

		double	MagnitudeSQ2d(const Vec &p) const;
		double	MagnitudeSQ2d(void) const;
		double	Magnitude2d(const Vec &p) const;
		double	Magnitude2d(void) const;
		// double	cos_angle(const Vec &a, const Vec &b);

		
		void	RotationZ(double rad);
		void	RotationX(double rad);
		void	RotationY(double rad);

		bool	operator==(const Vec &vec) const;
		Vec&	operator=(const Vec &vec);
		Vec		operator+(const Vec &vec);
		void	operator+=(const Vec &vec);
		Vec		operator-(const Vec &vec) const;
		Vec		operator*(const Vec &vec);
		Vec		operator*(const double a);
		void	operator*=(const double a);
		Vec		operator/(const Vec &vec);
};

std::ostream &operator<<(std::ostream &ostrm, const Vec &Vec);

#endif