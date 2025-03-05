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

		std::deque<Vec> intersectionWithMidHeight;

		Vec	n;
		
		Circle	circumcircle;

		bool	tempVertexFlg;

		Triangle();
		~Triangle();

		Triangle(const Vec &A, const Vec &B,const Vec &C, bool TempVertexFlg);

		bool	IsInsideCircumcircle(const Vec &point) const;
		void	CalcCircumcircle();
		double	CalcDistanceFromCenterSQ(const Vec &point) const;
		bool	InternalAndExternalJudgments2d(const Vec &point) const;
		bool	InternalAndExternalJudgments3d(const Vec &point) const;
		void	CalcNormalVector();
		// void	CalcIntersectionWithMidHeight(const Vec &midHeight);
		double	FindZ(const double pX, const double pY);

		void	DrawTriangle(const int64_t	maxHeight,
							const int64_t	minHeight,
							const double	midHeight,
							const bool	lineFlg, 
							const int64_t heightA, 
							const int64_t heightB, 
							const int64_t heightC);
		
		void	MoveVertexToMapCenter(const Vec &halfMapSize, const double midHeight);
		void	Rotation(void);

		bool		operator==(const Triangle &triangle) const;
		Triangle&	operator=(const Triangle &triangle);
		void		operator-=(const double num);
		void		operator-=(const Vec &vec);
};

std::ostream &operator<<(std::ostream &ostrm, const Triangle &triangle);

#endif