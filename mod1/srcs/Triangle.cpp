#include "../includes/Triangle.hpp"
#include "../includes/MathUtils.hpp"

Triangle::Triangle()
{

}

Triangle::Triangle(Vec A, Vec B, Vec C, bool TempVertexFlg):a(A), b(B), c(C), tempVertexFlg(TempVertexFlg)
{
	this->CalcCircumcircle();

	// std::cout << this->cOfCircumcircle[1] << std::endl;
	// std::cout << this->rOfCircumcircle << std::endl;
}

Triangle::~Triangle()
{

}

double	Triangle::CalcDistanceFromCenterSQ(const Vec &point)
{
	return point.MagnitudeSQ2d(this->circumcircle.center);
}

bool	Triangle::IsInsideCircumcircle(const Vec &point)
{
	return  this->CalcDistanceFromCenterSQ(point) + EPS < this->circumcircle.r;
}


void	calcMidPoint(const Vec &a, const Vec &b, double midPoint[2])
{
	midPoint[0]	= (a.x + b.x) / 2.0;
	midPoint[1]	= (a.y + b.y) / 2.0;

	// std::cout << "x " << midPoint[0]  << ", y " << midPoint[1] << std::endl;
}

void	calcLinearEquation(const Vec &a, const Vec &b, double	coeff[2], double &constNum)
{
	double	midPoint[2];

	calcMidPoint(a, b, midPoint);
	double	xCoeff = b.y - a.y;
	double	yCoeff = -(b.x - a.x);

	coeff[0] = -yCoeff;
	coeff[1] = xCoeff;

	// constNum = b.y * a.x - a.y * b.x;
	// std::cout << xCoeff << "x - " << yCoeff << "y = " << constNum << std::endl;

	constNum = coeff[0] * midPoint[0] + coeff[1] * midPoint[1];


	// std::cout << coeff[0] << "x - " << coeff[1] << "y = " << constNum << std::endl;
}

void	Triangle::CalcCircumcircle()
{
	double	abCoeff[2];
	double	acCoeff[2];

	double	abConstNum;
	double	acConstNum;

	calcLinearEquation(this->a, this->b, abCoeff, abConstNum);
	calcLinearEquation(this->a, this->c, acCoeff, acConstNum);

	double	det = abCoeff[0] * acCoeff[1] - acCoeff[0] * abCoeff[1];

	if (det == 0.0)
	{
		std::cout << "det is 0" << std::endl;
	}

	this->circumcircle.center.x = (abConstNum * acCoeff[1] - acConstNum * abCoeff[1]) / det;
	this->circumcircle.center.y = (abCoeff[0] * acConstNum - acCoeff[0] * abConstNum) / det;
	this->circumcircle.r = this->a.MagnitudeSQ2d(this->circumcircle.center);
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

void	Triangle::CalcNormalVector()
{
	this->n.x = (this->b.y - this->a.y) * (this->c.z - this->a.z) - 
				(this->b.z - this->a.z) * (this->c.y - this->a.y);
	this->n.y = (this->b.z - this->a.z) * (this->c.x - this->a.x) - 
				(this->b.x - this->a.x) * (this->c.z - this->a.z);
	this->n.z = (this->b.x - this->a.x) * (this->c.y - this->a.y) - 
				(this->b.y - this->a.y) * (this->c.x - this->a.x);
}

double	Triangle::FindZ(const double pX, const double pY)
{
	return this->a.z - (this->n.x * (pX - this->a.x) + this->n.y * (pY - this->a.y)) / this->n.z;
}

// void	Triangle::CalcIntersectionWithMidHeight(const Vec &midHeight)
// {
// 	if (this->a)
// }

bool	Triangle::operator==(const Triangle &triangle) const
{
	return (this->a == triangle.a) && 
		   (this->b == triangle.b) &&
		   (this->c == triangle.c) &&
		   (this->intersectionWithMidHeight == this->intersectionWithMidHeight) &&
		   (this->tempVertexFlg == triangle.tempVertexFlg);
}

Triangle&	Triangle::operator=(const Triangle &triangle)
{
	if (this != &triangle)
	{
		this->a = triangle.a;
		this->b = triangle.b;
		this->c = triangle.c;
		this->circumcircle = triangle.circumcircle;
		this->intersectionWithMidHeight = this->intersectionWithMidHeight;
		this->tempVertexFlg = triangle.tempVertexFlg;
	}
	return *this;
}

void		Triangle::operator-=(const double num)
{
	this->a -= num;
	this->b -= num;
	this->c -= num;
}

void		Triangle::operator-=(const Vec &vec)
{
	this->a -= vec;
	this->b -= vec;
	this->c -= vec;
}

std::ostream &operator<<(std::ostream &ostrm, const Triangle &triangle)
{
	return ostrm << '(' << triangle.a << ", " 
						<< triangle.b << ", "
						<< triangle.c << ')' 
						<< std::endl;
}