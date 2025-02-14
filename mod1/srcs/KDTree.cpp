
#include <iostream>
#include <string.h>

#include "../includes/KDTree.hpp"

KDTree::KDTree()
{

}

KDTree::KDTree(const uint32_t mapSize[2], std::deque<Vec> &specificPoints): checkIndex(NULL)
{
	this->CreateTree(mapSize, specificPoints);
}

KDTree::~KDTree()
{
	if (this->checkIndex)
	{
		delete(this->checkIndex);
	}
}

void	KDTree::CreateTree(const uint32_t mapSize[2], 
				   		   std::deque<Vec> &specificPoints)
{
	
	for (uint32_t x = 0; x < mapSize[X]; ++x)
	{
		if (x == 0 || x == mapSize[X] - 1)
		{
			for (uint32_t y = 0; y < mapSize[Y]; ++y)
			{
				this->_tree.insert(std::make_pair(x, y));
			}
		}
		else
		{
			this->_tree.insert(std::make_pair(x, 0));
			this->_tree.insert(std::make_pair(x, mapSize[Y] - 1));
		}
	}

	for (size_t	i = 0; i < specificPoints.size(); ++i)
	{
		this->_tree.insert(std::make_pair(specificPoints[i].x, specificPoints[i].y));
	}

	for (size_t	i = 0; i < this->_tree.size(); ++i)
	{
		Print::Out(std::to_string(this->_tree[i]));
	}

	this->checkIndex = new bool(this->_tree.size());
	memset(this->checkIndex, false, sizeof(this->checkIndex));
}

void	KDTree::ListUpNeighborPoints(size_t	  i,
									 uint32_t px,
									 uint32_t py,
									 uint32_t treeDepth,
								  	 int32_t  **&neighborPoints)
{
	if (this->checkIndex[i] == true)
	{
		return ;
	}

	this->checkIndex[i] = true;

	treeDepth += 1;
	this->ListUpNeighborPoints(i / 2, px, py, treeDepth, neighborPoints);

	if (treeDepth % 2 == 1) // odd
	{
		this->_tree.
	}
	else // even
	{

	}


}