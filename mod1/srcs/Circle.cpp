#include <GL/glut.h>
#include <GL/freeglut.h>
#include <cmath>

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

void Circle::_DrawCircle(const Vec &halfMapSize,
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

void Circle::DrawCircle2d(const Vec &halfMapSize,
							const double midHeight,
							const size_t num_segments) 
{
	Vec	c;

	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 1.0, 0.0);

	this->_DrawCircle(halfMapSize, midHeight, num_segments, c);

	glEnd();
}

void drawSphere(float radius, int slices, int stacks) {
    for (int i = 0; i < stacks; ++i) {
        float theta1 = i * M_PI / stacks;
        float theta2 = (i + 1) * M_PI / stacks;

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float phi = j * 2.0f * M_PI / slices;

            float x1 = radius * sin(theta1) * cos(phi);
            float y1 = radius * cos(theta1);
            float z1 = radius * sin(theta1) * sin(phi);

            float x2 = radius * sin(theta2) * cos(phi);
            float y2 = radius * cos(theta2);
            float z2 = radius * sin(theta2) * sin(phi);

            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }
}

void Circle::DrawCircle3d(const Vec &halfMapSize,
							const double midHeight,
							const size_t num_segments) 
{
	double theta;
	double phi;
	Vec	north;
	Vec	south;
	const size_t	halfSegments = num_segments / 2 + 1;

	glBegin(GL_LINE_STRIP);
	glColor3f(0.5, 0.5, 1);

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
	// Print::OutWords(this->center, north, south);
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
		this->rSQ = c.rSQ;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &ostrm, const Circle &c)
{
	return ostrm << '(' << c.center << ", " 
						<< c.r << ')' ;
}
