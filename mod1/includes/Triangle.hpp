#ifndef Triangle_HPP
# define Triangle_HPP

#include <iostream>
#include <string>
#include "./Vec.hpp"


class Triangle
{
	private:

	public:
		Vec a;
		Vec b;
		Vec c;

		Triangle();
		~Triangle();

		Triangle(Vec A, Vec B, Vec C);

		bool	InternalAndExternalJudgments(const Vec &point);

		bool	operator==(const Triangle &triangle) const;
		Triangle&	operator=(const Triangle &triangle);
};

std::ostream &operator<<(std::ostream &ostrm, const Triangle &triangle);

#endif