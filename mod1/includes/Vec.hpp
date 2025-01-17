#ifndef VEC_HPP
# define VEC_HPP

#include <iostream>
#include <string>

#define	MAX_COORDINATE 1000000
#define	MIN_COORDINATE 0

class Vec
{
	private:
		Vec();

	public:
		int32_t x;
		int32_t y;
		int32_t z;

		~Vec();

		Vec(int32_t X, int32_t Y, int32_t Z);
};

std::ostream &operator<<(std::ostream &ostrm, const Vec &Vec);

#endif