#include <GL/glut.h>
#include <GL/freeglut.h>
#include <cmath>
#include <omp.h>

#include "../includes/Circle.hpp"
#include "../includes/Graphic.hpp"
#include "../includes/Print.hpp"
#include "../includes/Utils.hpp"

Circle::Circle():center(0,0,0) ,r(0), rSQ(0)
{

}

Circle::Circle(const double &cX, 
			   const double &cY, 
			   const double &cZ, 
			   const double &_r,
			   const double &_rSQ):center(cX,cY,cZ) ,r(_r), rSQ(_rSQ)
{

}

Circle::Circle(const Circle &c)
{
	*this = c;
}

Circle::~Circle()
{

}

void	Circle::_DrawCircle(const Vec &halfMapSize,
						const double midHeight,
						const size_t num_segments, Vec &c)
{
	double	theta;
	double	x;
	double	y;

	for (size_t i = 0; i < num_segments; ++i)
	{
		theta = 2.0 * M_PI * double(i) / double(num_segments);
		x = this->r * cos(theta);
		y = this->r * sin(theta);
		c.x = this->center.x + x;
		c.y = this->center.y + y;
		drawVertex(move_vec_to_map_center(c, halfMapSize, midHeight));
	}
}

void	Circle::DrawCircle2d(const Vec &halfMapSize,
							const double midHeight,
							const size_t num_segments) 
{
	Vec	c;

	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 1.0, 0.0);

	this->_DrawCircle(halfMapSize, midHeight, num_segments, c);

	glEnd();
}

void	Circle::DrawCircle3d(const Vec &halfMapSize,
							const double midHeight,
							const size_t num_segments) 
{
	double theta;
	double phi;
	Vec	north;
	Vec	south;
	const size_t	halfSegments = num_segments / 2 + 1;

	glBegin(GL_LINE_STRIP);
	glColor4f(this->color.x, this->color.y, this->color.z, 0.6f);

	for (size_t i = 0; i < num_segments; ++i)
	{
		theta = i * M_PI / num_segments;
		for (size_t j = 0; j < halfSegments; ++j)
		{
			phi = j * 2.0f * M_PI / num_segments;

			north = this->center;
			south = this->center;

			north.x += this->r * sin(theta) * cos(phi);
			north.y += this->r * cos(theta);
			north.z += this->r * sin(theta) * sin(phi);

			south.x += this->r * sin(theta) * cos(-phi);
			south.y += this->r * cos(theta);
			south.z += this->r * sin(theta) * sin(-phi);

			drawVertex(move_vec_to_map_center(north, halfMapSize, midHeight));
			drawVertex(move_vec_to_map_center(south, halfMapSize, midHeight));
		}
	}
	glEnd();
}

void	Circle::DrawPoint3d(const Vec &halfMapSize, const double midHeight)
{
	drawVertex(move_vec_to_map_center(this->center, halfMapSize, midHeight));
}

bool	Circle::operator==(const Circle &c) const
{
	return (this->center == c.center) &&
		   (this->r == c.r);
}

Circle&	Circle::operator=(const Circle &c)
{
	if (this != &c)
	{
		this->center = c.center;
		this->color = c.color;
		this->r = c.r;
		this->rSQ = c.rSQ;
	}
	return *this;
}

std::ostream	&operator<<(std::ostream &ostrm, const Circle &c)
{
	return ostrm << '(' << c.center << ", " 
						<< c.r << ')' ;
}
