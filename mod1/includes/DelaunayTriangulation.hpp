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
		std::deque<Vec>			_specificPonints;
		std::deque<Triangle>	_triangles;
		std::deque<Side>		_sides;


		bool	*checkIndex;
		DT();

	public:
		DT(const std::deque<Vec> &specificPoints,
					 const uint32_t mapSize[2]);
		~DT();

		void	AddEndPonints(const uint32_t mapSize[2]);
		void	MakeSuperTriangle(const uint32_t mapSize[2]);
		void	SegmentTriangles(const Vec &specificPoint);
		void	Calculation();
};


#endif