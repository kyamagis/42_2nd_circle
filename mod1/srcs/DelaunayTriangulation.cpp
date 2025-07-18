#include <iostream>
#include "../includes/DelaunayTriangulation.hpp"
#include "../includes/Utils.hpp"

DT::DT()
{

}

DT::DT(const std::deque<Vec> &specificPoints, 
	   const uint32_t mapSize[3],
		const int64_t maxHeight,
		const int64_t minHeight): _specificPoints(specificPoints),
								  _maxHeight(maxHeight),
								  _minHeight(minHeight)
{
	this->_mapSize[X] = mapSize[X];
	this->_mapSize[Y] = mapSize[Y];
	this->_mapSize[Z] = mapSize[Z];

	this->_tempVertexB = Vec(2 * (this->_mapSize[X] - 1), 0, 0);
	this->_tempVertexC = Vec(0, 2 * (this->_mapSize[Y] - 1), 0);

	this->_MakeSuperTriangle();
}

DT::~DT()
{

}

void	DT::_MakeSuperTriangle(void)
{
	const Vec	origin(Vec(0, 0, 0));
	this->_triangles.push_back(Triangle(origin, 
								this->_tempVertexB, 
								this->_tempVertexC,
								true));

	this->_specificPoints.push_front(origin);
	this->_specificPoints.push_front(this->_tempVertexB);
	this->_specificPoints.push_front(this->_tempVertexC);
}

bool	DT::_HaveATempVertex(const Vec &a, const Vec &b, const Vec &c)
{
	if (a == this->_tempVertexB || 
		a == this->_tempVertexC)
	{
		return true ;
	}
	if (b == this->_tempVertexB || 
		b == this->_tempVertexC)
	{
		return true ;
	}
	if (c == this->_tempVertexB || 
		c == this->_tempVertexC)
	{
		return true ;
	}
	return false;
}

bool	DT::_AddSegmentedTriangle(const size_t idx, const Vec &a, const Vec &b)
{
	if (cross_product_2d(a, b, this->_specificPoints[idx]) == 0)
	{
		return false;
	}
	const bool	tempVertexFlg = this->_HaveATempVertex(a, b, this->_specificPoints[idx]);

	const Triangle t(a, b, this->_specificPoints[idx], tempVertexFlg);

	for (size_t i = 0; i < idx; ++i)
	{
		if (this->_specificPoints[i] == a ||
			this->_specificPoints[i] == b)
		{
			continue;
		}
		if (t.IsInsideCircumcircle(this->_specificPoints[i]))
		{
			return false;
		}
	}

	this->_triangles.push_back(t);
	return true;
}

void	DT::_SegmentTriangles(const size_t idx)
{
	const size_t	triangleSize = this->_triangles.size();
	std::deque<Triangle>::iterator itr = this->_triangles.begin();
	size_t	j = 0;

	for (size_t	i = 0; i < triangleSize; ++i)
	{
		if ((*itr).IsInsideCircumcircle(this->_specificPoints[idx]))
		{	
			this->_AddSegmentedTriangle(idx, (*itr).a, (*itr).b);
			this->_AddSegmentedTriangle(idx, (*itr).b, (*itr).c);
			this->_AddSegmentedTriangle(idx, (*itr).c, (*itr).a);
			itr = this->_triangles.erase(this->_triangles.begin() + j);
		}
		else
		{
			++j;
			++itr;
		}
	}
}

void	DT::_EraseTempTriangles(void)
{
	std::deque<Triangle>::iterator itr = this->_triangles.begin();

	for (; itr != this->_triangles.end();)
	{
		if (itr->tempVertexFlg)
		{	
			itr = this->_triangles.erase(itr);
		}
		else
		{
			(*itr).CalcNormalVector();
			++itr;
		}
	}
}

std::deque<Triangle>	DT::Calculation(void)
{
	const size_t	size = this->_specificPoints.size();

	for (size_t	i = 3; i < size; ++i)
	{
		this->_SegmentTriangles(i);
	}

	this->_EraseTempTriangles();
	Print::OutWords("Delaunay Triangulation Done");
	return this->_triangles;
}
