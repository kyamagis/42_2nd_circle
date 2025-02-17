#ifndef DelaunayTriangulation_HPP
# define DelaunayTriangulation_HPP

#include <deque>
#include <vector>

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
		uint32_t				_mapSize[2];

		Vec	_tempVertexB;
		Vec	_tempVertexC;

		DT();
		void	_AddEndPoints();
		void	_MakeSuperTriangle();
		void	_SegmentTriangles(const size_t idx);
		void	_AddSegmentedTriangle(const size_t idx, const Vec &a, const Vec &b);
		bool	_HaveATempVertex(const Vec &a, const Vec &b, const Vec &c);
		void	_SideFlip(const size_t idx, const Triangle &t);
		void	_EraseTempTriangles();
		void	_MakeMap(int64_t **map);

	public:
		DT(const std::deque<Vec> &specificPoints,
					 const uint32_t mapSize[2]);
		~DT();


		void	Calculation(int64_t **map);
		std::deque<Triangle>	Calculation(void);
};


#endif