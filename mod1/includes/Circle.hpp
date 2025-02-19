#ifndef Circle_HPP
# define Circle_HPP

#include <iostream>
#include <string>
#include "./Vec.hpp"

class Circle
{
	private:

	public:
		Vec		center;
		double	r;

		Circle();
		Circle(const double &X, const double &Y, const double &Z, const double &R);
		Circle(const Circle &c);
		virtual ~Circle();

		Circle&	operator=(const Circle &c);
		bool	operator==(const Circle &c) const;
};

std::ostream &operator<<(std::ostream &ostrm, const Circle &c);

#endif