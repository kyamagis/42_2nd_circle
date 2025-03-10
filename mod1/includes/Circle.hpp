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
		Circle(const double &cX, const double &cY, const double &cZ, const double &R);
		Circle(const Circle &c);
		virtual ~Circle();

		void DrawCircle2d(const uint32_t mapSize[3], 
							const Vec &halfMapSize,
							const double scaling, 
							const int num_segments);
		void DrawCircle3d(const uint32_t mapSize[3], 
							const Vec &halfMapSize,
							const double scaling,
							const int num_segments);
		Circle&	operator=(const Circle &c);
		bool	operator==(const Circle &c) const;
};

std::ostream &operator<<(std::ostream &ostrm, const Circle &c);

#endif