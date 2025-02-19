#ifndef Triangle_HPP
# define Triangle_HPP

#include <deque>
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

		std::deque<Vec> intersectionWithMidHeight;

		Vec	n;
		
		Circle	circumcircle;

		bool	tempVertexFlg;

		Triangle();
		~Triangle();

		Triangle(Vec A, Vec B, Vec C, bool TempVertexFlg);

		bool	IsInsideCircumcircle(const Vec &point);
		void	CalcCircumcircle();
		double	CalcDistanceFromCenterSQ(const Vec &point);
		bool	InternalAndExternalJudgments(const Vec &point);
		void	CalcNormalVector();
		void	CalcIntersectionWithMidHeight(const int64_t maxHeight, 
											  const int64_t minHeight);
		double	FindZ(const double pX, const double pY);

		bool	operator==(const Triangle &triangle) const;
		Triangle&	operator=(const Triangle &triangle);
};

std::ostream &operator<<(std::ostream &ostrm, const Triangle &triangle);

#endif