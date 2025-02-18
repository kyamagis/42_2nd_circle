#ifndef ParticleMethod_HPP
# define ParticleMethod_HPP

#include <iostream>
#include <string>
#include "./Vec.hpp"


class PT
{
	private:

	public:
		double		x;
		double		y;
		double		r;

		PT();
		PT(const PT &pT);
		~PT();

		PT&	operator=(const PT &pT);
		bool	operator==(const PT &pT) const;
};

std::ostream &operator<<(std::ostream &ostrm, const PT &pT);

#endif