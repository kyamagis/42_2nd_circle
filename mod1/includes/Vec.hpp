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
		~Vec();

		Vec(int32_t X, int32_t Y, int32_t Z);
		Vec(const Vec &vec);

		bool	operator==(const Vec &vec) const;
		Vec&	operator=(const Vec &vec);
};

std::ostream &operator<<(std::ostream &ostrm, const Vec &Vec);

#endif