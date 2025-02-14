#ifndef ThreeDSpline_HPP
# define ThreeDSpline_HPP

#include <deque>
#include <map>

#include "./Vec.hpp"
#include "./TerrestrialSimulation.hpp"
#include "../includes/Print.hpp"

class ThreeDSpline
{
	private:
		std::deque<Vec>	_specificPonints;
		bool	*checkIndex;
		ThreeDSpline();

	public:
		ThreeDSpline(const std::deque<Vec> &specificPoints,
					 const uint32_t mapSize[2]);
		~ThreeDSpline();

		void	CreateSpecificPonints(const uint32_t mapSize[2]);

		void ListUpNeighborPoints(size_t	  i,
									uint32_t px,
									uint32_t py,
									uint32_t treeDepth,
								  	int32_t  **&neighborPoints);
};


#endif