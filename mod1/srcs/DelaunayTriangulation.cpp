#include <iostream>
#include <iomanip>
#include "../includes/DelaunayTriangulation.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Graphic.hpp"

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

	this->_tempVertexB = Vec(2 * this->_mapSize[X], 0, 0);
	this->_tempVertexC = Vec(0, 2 * this->_mapSize[Y], 0);

	this->_MakeSuperTriangle();
}

DT::~DT()
{

}

void	DT::_MakeSuperTriangle(void)
{
	Vec	o(Vec(0, 0, 0));
	this->_triangles.push_back(Triangle(o, 
								this->_tempVertexB, 
								this->_tempVertexC,
								true));

	this->_specificPoints.push_front(o);
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
	bool	tempVertexFlg = this->_HaveATempVertex(a, b, this->_specificPoints[idx]);

	const Triangle t(a, b, this->_specificPoints[idx], tempVertexFlg);

	for (size_t i = 0; i <= idx; ++i)
	{
		if (this->_specificPoints[i] == a 
			|| this->_specificPoints[i] == b
			|| idx == i)
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

void	DT::_MakeMap(int64_t **map)
{
	size_t	i = 0;
	size_t	j = 0;
	size_t	searchedI = 0;
	size_t	total = this->_mapSize[X] * this->_mapSize[Y];

	for (size_t	x = 0; x < this->_mapSize[X]; ++x)
	{
		for (size_t	y = 0; y < this->_mapSize[Y]; ++y)
		{
			if (this->_triangles[i].InternalAndExternalJudgments2d(Vec(x, y, 0)))
			{
				map[x][y] = this->_triangles[i].FindZ(x, y);
			}
			else
			{
				searchedI = i;
				for (i = 0; i < this->_triangles.size(); ++i)
				{
					if (i == searchedI)
					{
						continue;
					}
					if (this->_triangles[i].InternalAndExternalJudgments2d(Vec(x, y, 0)))
					{
						map[x][y] = this->_triangles[i].FindZ(x, y);
						break;
					}
	
				}
			}

			// for (i = 0; i < this->_triangles.size(); ++i)
			// {
			// 	if (this->_triangles[i].InternalAndExternalJudgments(Vec(x, y, 0)))
			// 	{
			// 		map[x][y] = this->_triangles[i].FindZ(x, y);
			// 		break;
			// 	}

			// }

			std::cout << std::fixed << std::setprecision(1) 
				  << double(j) /total * 100
				  << " %\r" << std::flush;
			++j;
		}
	}
}

void	DT::Calculation(int64_t **map)
{
	this->Calculation();

	this->_MakeMap(map);
}

std::deque<Triangle>	DT::Calculation(void)
{
	const size_t	size = this->_specificPoints.size();

	// i is 3, because (0,0,0) _tempVertexB, and _tempVertexC is top of this->_specificPoints
	for (size_t	i = 3; i < size; ++i)
	{
		this->_SegmentTriangles(i);
		std::cout <<std::fixed << std::setprecision(1)
				  << double(i) / this->_specificPoints.size() * 100
				  << " %\r" << std::flush;
	}

	std::cout <<  std::endl << "Triangulation Done" << std::endl;

	this->_EraseTempTriangles();
	return this->_triangles;
}


// void	DT::SideFlip(const size_t idx, const Triangle &t)
// {
// 	double cosAPB = cos_angle(t.a, this->_specificPoints[idx], t.b);
// 	double cosBPC = cos_angle(t.b, this->_specificPoints[idx], t.c);
// 	double cosCPA = cos_angle(t.c, this->_specificPoints[idx], t.a);

// 	if (cosAPB <= cosBPC && cosAPB <= cosCPA) // c
// 	{
// 		std::cout << "side flip /" << this->_triangles.size() << std::endl;
// 		this->_AddSegmentedTriangle(idx, t.b, t.c);
// 		this->_AddSegmentedTriangle(idx, t.c, t.a);
// 		std::cout << "side flip /" << this->_triangles.size() << std::endl;
// 		return ;
// 	}
// 	if (cosBPC <= cosAPB && cosBPC <= cosCPA) // a
// 	{
// 		this->_AddSegmentedTriangle(idx, t.a, t.b);
// 		this->_AddSegmentedTriangle(idx, t.c, t.a);
// 		return ;
// 	}
// 	// b
// 	this->_AddSegmentedTriangle(idx, t.a, t.b);
// 	this->_AddSegmentedTriangle(idx, t.b, t.c);
// }