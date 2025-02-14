#include "../includes/Triangle.hpp"

Triangle::Triangle()
{

}

Triangle::Triangle(Vec A, Vec B, Vec C):a(A), b(B), c(C)
{
	
}

Triangle::~Triangle()
{

}

static int32_t	cross_product(const Vec &a, const Vec &b, const Vec &point)
{
	int32_t	a_to_bx = b.x - a.x;
	int32_t	a_to_by = b.y - a.y;

	int32_t	b_to_px = point.x - b.x;
	int32_t b_to_py = point.y - b.y;

	return (a_to_bx * b_to_py) - (a_to_by * b_to_px);
}

bool	Triangle::InternalAndExternalJudgments(const Vec &point)
{
	if (this->a == this->b || 
		this->b == this->c || 
		this->c == this->a)
		return false;

	int32_t	abp = cross_product(this->a, this->b, point);
	int32_t	bcp = cross_product(this->b, this->c, point);
	if ((abp ^ bcp) < 0)
	{
		return false ;
	}
	int32_t	cap = cross_product(this->c, this->a, point);
	return (0 <= (abp ^ cap));
}

bool	Triangle::operator==(const Triangle &triangle) const
{
	return (this->a == triangle.a) && 
		   (this->b == triangle.b) &&
		   (this->c == triangle.c);
}

Triangle&	Triangle::operator=(const Triangle &triangle)
{
	if (this != &triangle)
	{
		this->a = triangle.a;
		this->b = triangle.b;
		this->c = triangle.c;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &ostrm, const Triangle &triangle)
{
	return ostrm << '(' << triangle.a << ", " 
						<< triangle.b << ", "
						<< triangle.c << ')' 
						<< std::endl;
}