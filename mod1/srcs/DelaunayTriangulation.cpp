#include <iostream>
#include <iomanip>
#include "../includes/DelaunayTriangulation.hpp"
#include "../includes/MathUtils.hpp"
#include "../includes/Graphic.hpp"

#define OMMIT 1000

DT::DT()
{

}

DT::DT(const std::deque<Vec> &specificPoints, 
	   const uint32_t mapSize[2]): _specificPoints(specificPoints)
{
	this->_tempVertexB = Vec(2 * mapSize[X], 0, 0);
	this->_tempVertexC = Vec(0, 2 * mapSize[Y], 0);

	this->AddEndPoints(mapSize);
	this->MakeSuperTriangle();
}

DT::~DT()
{

}

void	DT::MakeSuperTriangle()
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

void	DT::AddEndPoints(const uint32_t mapSize[2])
{
	for (uint32_t x = 0; x < mapSize[X]; ++x)
	{
		if (x == 0 || x == mapSize[X] - 1)
		{
			for (uint32_t y = 0; y < mapSize[Y]; ++y)
			{
				if (x == 0 && y == 0)
				{
					continue;
				}
				if (y % OMMIT == 0 || y + 1 ==  mapSize[Y])
				{
					this->_specificPoints.push_back(Vec(x, y, 0));
				}
			}
		}
		else if (x % OMMIT == 0)
		{
			this->_specificPoints.push_back(Vec(x, 0, 0));
			this->_specificPoints.push_back(Vec(x, mapSize[Y] - 1, 0));
		}
	}
}

bool	DT::HaveATempVertex(const Vec &a, const Vec &b, const Vec &c)
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

void	DT::AddSegmentedTriangle(const size_t idx, const Vec &a, const Vec &b)
{
	if (cross_product(a, b, this->_specificPoints[idx]) == 0)
	{
		return ;
	}
	bool	tempVertexFlg = this->HaveATempVertex(a, b, this->_specificPoints[idx]);

	Triangle t(a, b, this->_specificPoints[idx], tempVertexFlg);

	for (size_t i = 0; i <= idx; ++i)
	{
		if (this->_specificPoints[i] == a 
			|| this->_specificPoints[i] == b
			|| idx == i)
		{
			continue;
		}
		if (t.IsInsideCircumcircle(this->_specificPoints[i].x, 
								   this->_specificPoints[i].y))
		{
			return ;
		}
	}

	this->_triangles.push_back(t);
}

void	DT::SegmentTriangles(const size_t idx)
{
	size_t		triangleSize = this->_triangles.size();
	std::deque<Triangle>::iterator itr = this->_triangles.begin();
	size_t	j = 0;

	for (size_t	i = 0; i < triangleSize; ++i)
	{
		if ((*itr).IsInsideCircumcircle(this->_specificPoints[idx].x, 
										this->_specificPoints[idx].y))
		{	
			this->AddSegmentedTriangle(idx, (*itr).a, (*itr).b);
			this->AddSegmentedTriangle(idx, (*itr).b, (*itr).c);
			this->AddSegmentedTriangle(idx, (*itr).c, (*itr).a);
			itr = this->_triangles.erase(this->_triangles.begin() + j);
		}
		else
		{
			++j;
			++itr;
		}
	}
	// std::cout << "-------------" << this->_triangles.size() << std::endl;
}

void	DT::EraseTempTriangles()
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
			++itr;
		}
	}
}

std::deque<Triangle>	DT::Calculation()
{
	size_t	size = this->_specificPoints.size();
	// size_t	size = 23;

	// i is 3, because (0,0,0) _tempVertexB, and _tempVertexC is top of this->_specificPoints
	for (size_t	i = 3; i < size; ++i)
	{
		this->SegmentTriangles(i);
		std::cout << std::fixed << std::setprecision(1) 
				  << double(i) / this->_specificPoints.size() * 100
				  << " %\r" << std::flush;
	}

	this->EraseTempTriangles();

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
// 		this->AddSegmentedTriangle(idx, t.b, t.c);
// 		this->AddSegmentedTriangle(idx, t.c, t.a);
// 		std::cout << "side flip /" << this->_triangles.size() << std::endl;
// 		return ;
// 	}
// 	if (cosBPC <= cosAPB && cosBPC <= cosCPA) // a
// 	{
// 		this->AddSegmentedTriangle(idx, t.a, t.b);
// 		this->AddSegmentedTriangle(idx, t.c, t.a);
// 		return ;
// 	}
// 	// b
// 	this->AddSegmentedTriangle(idx, t.a, t.b);
// 	this->AddSegmentedTriangle(idx, t.b, t.c);
// }