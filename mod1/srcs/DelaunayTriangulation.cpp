#include <iostream>

#include "../includes/DelaunayTriangulation.hpp"

DT::DT(const std::deque<Vec> &specificPoints, 
	   const uint32_t mapSize[2]): _specificPonints(specificPoints)
{
	this->AddEndPonints(mapSize);
	this->MakeSuperTriangle(mapSize);
}

DT::~DT()
{

}

void	DT::MakeSuperTriangle(const uint32_t mapSize[2])
{
	this->_triangles.push_back({Vec(0, 0, 0), 
								Vec(2 * mapSize[X], 0, 0), 
								Vec(0, 2 * mapSize[Y], 0)});
}

void	DT::AddEndPonints(const uint32_t mapSize[2])
{
	for (uint32_t x = 0; x < mapSize[X]; ++x)
	{
		if (x == 0 || x == mapSize[X] - 1)
		{
			for (uint32_t y = 0; y < mapSize[Y]; ++y)
			{
				this->_specificPonints.push_back(Vec(x, y, 0));
			}
		}
		else
		{
			this->_specificPonints.push_back(Vec(x, 0, 0));
			this->_specificPonints.push_back(Vec(x, mapSize[Y] - 1, 0));
		}
	}

	for (size_t	i = 0; i < this->_specificPonints.size(); ++i)
	{
		std::cout << this->_specificPonints[i] << std::endl;
	}
}

void	DT::SegmentTriangles(const Vec &specificPoint)
{
	// Triangle	segmentedTriangls[3];
	size_t		triangleSize = this->_triangles.size();

	for (size_t	i = 0; i < triangleSize; ++i)
	{
		if (this->_triangles[i].InternalAndExternalJudgments(specificPoint))
		{
			this->_triangles.push_back(Triangle(this->_triangles[i].a, this->_triangles[i].b, specificPoint));
			this->_triangles.push_back(Triangle(this->_triangles[i].b, this->_triangles[i].c, specificPoint));
			this->_triangles.push_back(Triangle(this->_triangles[i].c, this->_triangles[i].a, specificPoint));
		}
	}
}

void	DT::Calculation()
{
	for (size_t	i = 0; i < this->_specificPonints.size(); ++i)
	{
		this->SegmentTriangles(this->_specificPonints[i]);
	}

	this->_triangles.pop_front();
}