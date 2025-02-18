#ifndef Triangle_HPP
# define Triangle_HPP

#include <iostream>
#include <string>
#include "./Vec.hpp"
#include "./Circle.hpp"


class Triangle
{
	private:

	public:
		Vec a;
		Vec b;
		Vec c;

		Vec	n;
		
		Circle	circumcircle;

		bool	tempVertexFlg;

		Triangle();
		~Triangle();

		Triangle(Vec A, Vec B, Vec C, bool TempVertexFlg);

		bool	IsInsideCircumcircle(double x, double y);
		void	CalcCircumcircle();
		double	CalcDistanceFromCenterSQ(double x, double y);
		bool	InternalAndExternalJudgments(const Vec &point);
		void	CalcNormalVector();
		double	FindZ(const double pX, const double pY);

		bool	operator==(const Triangle &triangle) const;
		Triangle&	operator=(const Triangle &triangle);
};

std::ostream &operator<<(std::ostream &ostrm, const Triangle &triangle);

#endif