#ifndef Circle_HPP
# define Circle_HPP

#include <iostream>
#include <string>
#include "./Vec.hpp"


class Circle
{
	private:

	public:
		double		x;
		double		y;
		double		r;

		Circle();
		Circle(const Circle &circle);
		~Circle();

		Circle&	operator=(const Circle &circle);
		bool	operator==(const Circle &circle) const;
};

std::ostream &operator<<(std::ostream &ostrm, const Circle &circle);

#endif