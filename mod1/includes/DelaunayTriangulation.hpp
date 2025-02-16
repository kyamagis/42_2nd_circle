#ifndef DelaunayTriangulation_HPP
# define DelaunayTriangulation_HPP

#include <deque>
#include <map>

#include "./Vec.hpp"
#include "./Triangle.hpp"
#include "./Side.hpp"
#include "./TerrestrialSimulation.hpp"
#include "../includes/Print.hpp"

class DT
{
	private:
		std::deque<Vec>			_specificPoints;
		std::deque<Triangle>	_triangles;
		std::deque<Side>		_sides;

		Vec	_tempVertexB;
		Vec	_tempVertexC;


		bool	*checkIndex;
		DT();

	public:
		DT(const std::deque<Vec> &specificPoints,
					 const uint32_t mapSize[2]);
		~DT();

		void	AddEndPoints(const uint32_t mapSize[2]);
		void	MakeSuperTriangle();
		void	SegmentTriangles(const size_t idx);
		void	AddSegmentedTriangle(const size_t idx, const Vec &a, const Vec &b);
		bool	HaveATempVertex(const Vec &a, const Vec &b, const Vec &c);
		void	SideFlip(const size_t idx, const Triangle &t);

		void	EraseTempTriangles();

		std::deque<Triangle>	Calculation();
};


#endif