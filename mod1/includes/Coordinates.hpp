#ifndef COORDINATES_HPP
# define COORDINATES_HPP

#include <iostream>
#include <string>

#define	MAX_COORDINATE 1000000
#define	MIN_COORDINATE 0

class Coordinates
{
	private:
		Coordinates()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}

	public:
		int32_t x;
		int32_t y;
		int32_t z;

		~Coordinates() {}

		Coordinates(int32_t x, int32_t y, int32_t z):x(x), y(y), z(z) {}
};

std::ostream &operator<<(std::ostream &ostrm, const Coordinates &coordinates);

#endif