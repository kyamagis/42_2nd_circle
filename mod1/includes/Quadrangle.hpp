#ifndef Quadrangle_HPP
# define Quadrangle_HPP

#include <deque>
#include <iostream>
#include <string>
#include "./Vec.hpp"
#include "./Circle.hpp"

class Quadrangle
{
	private:

	public:
		Vec	vertex[4];
		Vec	n;

		Quadrangle();
		~Quadrangle();

		Quadrangle(const Vec &a, const Vec &b, const Vec &c, const Vec &d);

		void	DrawQuad(void) const;

		bool		operator==(const Quadrangle &q) const;
		Quadrangle&	operator=(const Quadrangle &q);
		void		operator-=(const double num);
		void		operator-=(const Quadrangle &q);
};

std::ostream &operator<<(std::ostream &ostrm, const Quadrangle &q);

#endif