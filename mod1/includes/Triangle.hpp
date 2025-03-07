#ifndef TRIANGLE_HPP
# define TRIANGLE_HPP

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
		Vec	n;
		

		Circle	circumcircle;

		bool	tempVertexFlg;
		bool	visibleFlg;

		Triangle();
		~Triangle();

		Triangle(const Vec &A, const Vec &B,const Vec &C, const bool TempVertexFlg);
		Triangle(const Vec &A, const Vec &B,const Vec &C, const bool TempVertexFlg, const bool _visibleFlg);

		bool	IsInsideCircumcircle(const Vec &point) const;
		void	CalcCircumcircle();
		double	CalcDistanceFromCenterSQ(const Vec &point) const;
		bool	InternalAndExternalJudgments2d(const Vec &point) const;
		bool	InternalAndExternalJudgments3d(const Vec &point) const;
		void	CalcNormalVector();
		double	FindZ(const double pX, const double pY);

		void	DrawFrame(const double	midHeight, const Vec &halfMapSize);
		void	DrawTriangle(const int64_t	maxHeight,
							const int64_t	minHeight,
							const double	midHeight,
							const Vec &halfMapSize,
							const bool	lineFlg);
		
		void	MoveVertexToMapCenter(const Vec &halfMapSize, const double midHeight);

		bool		operator==(const Triangle &triangle) const;
		Triangle&	operator=(const Triangle &triangle);
		void		operator-=(const double num);
		void		operator-=(const Vec &vec);
};

std::ostream &operator<<(std::ostream &ostrm, const Triangle &triangle);

#endif