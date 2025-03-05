#include "../includes/Triangle.hpp"
#include "../includes/Utils.hpp"

Triangle::Triangle()
{

}

Triangle::Triangle(const Vec &A, const Vec &B,const Vec &C, bool TempVertexFlg):a(A), b(B), c(C), tempVertexFlg(TempVertexFlg)
{
	this->CalcCircumcircle();

	// std::cout << this->cOfCircumcircle[1] << std::endl;
	// std::cout << this->rOfCircumcircle << std::endl;
}

Triangle::~Triangle()
{

}

double	Triangle::CalcDistanceFromCenterSQ(const Vec &point) const
{
	return point.MagnitudeSQ2d(this->circumcircle.center);
}

bool	Triangle::IsInsideCircumcircle(const Vec &point) const
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

bool	Triangle::InternalAndExternalJudgments2d(const Vec &point) const
{
	if (this->a == this->b || 
		this->b == this->c || 
		this->c == this->a)
		return false;

	int32_t	abp = cross_product_2d(this->a, this->b, point);
	int32_t	bcp = cross_product_2d(this->b, this->c, point);
	if ((abp ^ bcp) < 0)
	{
		return false ;
	}
	int32_t	cap = cross_product_2d(this->c, this->a, point);
	return (0 <= (abp ^ cap));
}

bool	Triangle::InternalAndExternalJudgments3d(const Vec &point) const
{
	if (this->a == this->b || 
		this->b == this->c || 
		this->c == this->a)
		return false;

	// Vec	abp = (this->b - this->a).CrossProduct3d(point - this->b);
	// abp /=  abp.Magnitude3d();
	// const Vec	bcp = (this->c - this->b).CrossProduct3d(point - this->c);
	// if (!abp.NearlyEqual(bcp / bcp.Magnitude3d()))
	// {
	// 	return false ;
	// }
	// const Vec	cap = (this->a - this->c).CrossProduct3d(point - this->a);
	// return (abp.NearlyEqual(cap / cap.Magnitude3d()));

	// const Vec	abp = (this->b - this->a).CrossProduct3d(point - this->b);
	// const Vec	bcp = (this->c - this->b).CrossProduct3d(point - this->c);
	// if (!abp.NearlyEqual(bcp))
	// {
	// 	return false ;
	// }
	// const Vec	cap = (this->a - this->c).CrossProduct3d(point - this->a);
	// return (abp.NearlyEqual(cap));

	// const Vec	abp = (this->a - this->b).CrossProduct3d(point - this->b);
	// const Vec	bcp = (this->b - this->c).CrossProduct3d(point - this->c);
	// const Vec	cap = (this->c - this->a).CrossProduct3d(point - this->a);

	// if (!abp.NearlyEqual(bcp))
	// {
	// 	return false ;
	// }
	// return (abp.NearlyEqual(cap));

	const Vec	abp = (this->a - this->b).CrossProduct3d(point - this->b);
	const Vec	bcp = (this->b - this->c).CrossProduct3d(point - this->c);
	const Vec	cap = (this->c - this->a).CrossProduct3d(point - this->a);

	double	adot = abp.DotProduct3d(this->n);
	double	bdot = bcp.DotProduct3d(this->n);
	double	cdot = cap.DotProduct3d(this->n);

	if (0.0 <= adot && 0.0 <= bdot && 0.0 <= cdot)
	{
		return true ;
	}
	else if (adot < 0.0 && bdot < 0.0 && cdot < 0.0)
	{
		return true;
	}
	return false;
}

void	Triangle::CalcNormalVector()
{
	this->n = (this->b - this->a).CrossProduct3d(this->c - this->a);
}

double	Triangle::FindZ(const double pX, const double pY)
{
	return this->a.z - (this->n.x * (pX - this->a.x) + this->n.y * (pY - this->a.y)) / this->n.z;
}

void	Triangle::DrawTriangle(const int64_t	maxHeight,
							const int64_t	minHeight,
							const double	midHeight,
							const bool	lineFlg, 
							const int64_t heightA, 
					 		const int64_t heightB, 
					 		const int64_t heightC)
{
	if (lineFlg)
	{
		glBegin(GL_LINE_LOOP);
	}
	else
	{
		glShadeModel(GL_SMOOTH);
	}

	glBegin(GL_TRIANGLES);

	line_gradation(maxHeight, minHeight, midHeight, heightA);
	drawVertex(this->a);
	line_gradation(maxHeight, minHeight, midHeight, heightB);
	drawVertex(this->b);
	line_gradation(maxHeight, minHeight, midHeight, heightC);
	drawVertex(this->c);
	glEnd();
}

void	Triangle::MoveVertexToMapCenter(const Vec &halfMapSize, const double midHeight)
{
	move_vec_to_map_center(this->a, halfMapSize, midHeight);
	move_vec_to_map_center(this->b, halfMapSize, midHeight);
	move_vec_to_map_center(this->c, halfMapSize, midHeight);
	this->circumcircle.center.x -= halfMapSize.x;
	this->circumcircle.center.y -= halfMapSize.y;
}

void	Triangle::Rotation(void)
{
	rotation(this->a);
	rotation(this->b);
	rotation(this->c);
}

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
		this->n = triangle.n;
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
						<< triangle.c << ')';
}