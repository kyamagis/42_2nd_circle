#ifndef CIRCLE_HPP
# define CIRCLE_HPP

#include <iostream>
#include <string>
#include "./Vec.hpp"

class Circle
{
	private:

	public:
		Vec		center;
		Vec		color;
		double	r;
		double	rSQ;

		Circle();
		Circle(const double &cX, const double &cY, const double &cZ, 
			   const double &_r, const double &_rSQ);
		Circle(const Circle &c);
		virtual ~Circle();

		void	_DrawCircle(const Vec &halfMapSize,
						const double midHeight,
						const size_t num_segments, Vec &c);
		void	DrawCircle2d(const Vec &halfMapSize,
							const double midHeight,
							const size_t num_segments);
		void	DrawCircle3d(const Vec &halfMapSize,
							const double midHeight,
							const size_t num_segments);
		void	DrawPoint3d(const Vec &halfMapSize, const double midHeight);

		Circle&	operator=(const Circle &c);
		bool	operator==(const Circle &c) const;
};

std::ostream &operator<<(std::ostream &ostrm, const Circle &c);

#endif