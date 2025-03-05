#include "../includes/Quadrangle.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Graphic.hpp"

Quadrangle::Quadrangle()
{

}

Quadrangle::Quadrangle(const Vec &a, const Vec &b, const Vec &c,const Vec &d)
{
	this->vertex[0] = a;
	this->vertex[1] = b;
	this->vertex[2] = c;
	this->vertex[3] = d;
}

Quadrangle::~Quadrangle()
{

}

void	Quadrangle::DrawQuad(void) const
{
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_LINE_LOOP);
	glBegin(GL_QUADS);
	drawVertex(this->vertex[0]);
	drawVertex(this->vertex[1]);
	drawVertex(this->vertex[2]);
	drawVertex(this->vertex[3]);
	glEnd();
}


bool	Quadrangle::operator==(const Quadrangle &q) const
{
	return (this->vertex[0] == q.vertex[0]) && 
		   (this->vertex[1] == q.vertex[1]) &&
		   (this->vertex[2] == q.vertex[2]) &&
		   (this->vertex[3] == q.vertex[3]);
}

Quadrangle&	Quadrangle::operator=(const Quadrangle &q)
{
	if (this != &q)
	{
		this->vertex[0] = q.vertex[0];
		this->vertex[1] = q.vertex[1];
		this->vertex[2] = q.vertex[2];
		this->vertex[3] = q.vertex[3];
	}
	return *this;
}

void		Quadrangle::operator-=(const double num)
{
	this->vertex[0] -= num;
	this->vertex[1] -= num;
	this->vertex[2] -= num;
	this->vertex[3] -= num;
}

void		Quadrangle::operator-=(const Quadrangle &q)
{
	this->vertex[0] -= q.vertex[0];
	this->vertex[1] -= q.vertex[1];
	this->vertex[2] -= q.vertex[2];
	this->vertex[3] -= q.vertex[3];
}

std::ostream &operator<<(std::ostream &ostrm, const Quadrangle &q)
{
	return ostrm << '(' << q.vertex[0] << ", " 
						<< q.vertex[1] << ", "
						<< q.vertex[2] << ", "
						<< q.vertex[3] << ')' 
						<< std::endl;
}