#ifndef KDTREE_HPP
# define KDTREE_HPP

#include <deque>
#include <map>

#include "./Vec.hpp"
#include "./TerrestrialSimulation.hpp"
#include "../includes/Print.hpp"

class KDTree
{
	private:
		std::map<uint32_t, uint32_t> _tree;
		bool	*checkIndex;
		KDTree();

	public:
		KDTree(const uint32_t mapSize[2], std::deque<Vec> &specificPoints);
		~KDTree();

		void	CreateTree(const uint32_t mapSize[2], 
				           std::deque<Vec> &specificPoints);

		void ListUpNeighborPoints(size_t	  i,
									uint32_t px,
									uint32_t py,
									uint32_t treeDepth,
								  	int32_t  **&neighborPoints);
};


#endif