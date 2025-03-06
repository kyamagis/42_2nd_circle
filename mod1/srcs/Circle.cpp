#include <GL/glut.h>
#include <GL/freeglut.h>
#include <cmath>

#include "../includes/Circle.hpp"

Circle::Circle():center(0,0,0) ,r(0)
{

}

Circle::Circle(const double &cX, 
			   const double &cY, 
			   const double &cZ, 
			   const double &R):center(cX,cY,cZ) ,r(R)
{

}

Circle::Circle(const Circle &c)
{
	*this = c;
}

Circle::~Circle()
{

}

void Circle::DrawCircle2d(const uint32_t mapSize[3], 
						const Vec &halfMapSize,
						const double scaling,
						const int num_segments) 
{
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 1.0, 0.0);

	double	cx = double(this->center.x - halfMapSize.x) / mapSize[X];
	double	cy = - 1.0 * (this->center.y - halfMapSize.y) / mapSize[Y];
	double 	r2d = sqrt(this->r) / mapSize[X];

	for (int i = 0; i < num_segments; i++) {
		double theta = 2.0 * M_PI * double(i) / double(num_segments); // 角度
		double x = r2d * cos(theta);
		double y = r2d * sin(theta);
		glVertex2d((cx + x) * scaling, (cy + y) * scaling);
	}
	glEnd();
}

void Circle::DrawCircle3d(const uint32_t mapSize[3], 
							const Vec &halfMapSize,
							const double scaling,
							const int num_segments) 
{
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 1.0, 0.0);

	double	cx = double(this->center.x - halfMapSize.x) / mapSize[X];
	double	cy = - 1.0 * (this->center.y - halfMapSize.y) / mapSize[Y];
	// double	cz = double(this->center.z) / mapSize[Z];
	double 	r2d = sqrt(this->r) / mapSize[X];

	for (int i = 0; i < num_segments; i++) {
		double theta = 2.0 * M_PI * double(i) / double(num_segments); // 角度
		double x = r2d * cos(theta);
		double y = r2d * sin(theta);
		glVertex2d((cx + x) * scaling, (cy + y) * scaling);
	}
	glEnd();
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
		this->r = c.r;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &ostrm, const Circle &c)
{
	return ostrm << '(' << c.center << ", " 
						<< c.r << ')' 
						<< std::endl;
}