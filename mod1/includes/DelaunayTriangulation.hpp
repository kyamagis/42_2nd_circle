#ifndef DelaunayTriangulation_HPP
# define DelaunayTriangulation_HPP

#include <deque>
#include <vector>

#include "./Vec.hpp"
#include "./Triangle.hpp"
#include "./TerrestrialSimulation.hpp"
#include "../includes/Print.hpp"

class DT
{
	private:
		std::deque<Vec>			_specificPoints;
		uint32_t				_mapSize[3];
		int64_t					_maxHeight;
		int64_t					_minHeight;
		std::deque<Triangle>	_triangles;

		Vec	_tempVertexB;
		Vec	_tempVertexC;

		DT();
		void	_AddEndPoints(void);
		void	_MakeSuperTriangle(void);
		bool	_AddSegmentedTriangle(const size_t idx, const Vec &a, const Vec &b);
		void	_SegmentTriangles(const size_t idx);
		bool	_HaveATempVertex(const Vec &a, const Vec &b, const Vec &c);
		void	_SideFlip(const size_t idx, const Triangle &t);
		void	_EraseTempTriangles(void);
		void	_IntegrateTriangles(void);

	public:
		DT(const std::deque<Vec> &specificPoints,
		   const uint32_t mapSize[3], 
		   const int64_t maxHeight,
		   const int64_t minHeight);
		~DT();

		std::deque<Triangle>	Calculation(void);
};


#endif