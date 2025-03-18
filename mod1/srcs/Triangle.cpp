#include "../includes/Triangle.hpp"
#include "../includes/Utils.hpp"

Triangle::Triangle()
{

}

Triangle::Triangle(const Vec &A, const Vec &B,const Vec &C, const bool TempVertexFlg): 
					a(A), b(B), c(C), tempVertexFlg(TempVertexFlg), visibleFlg(true)
{
	this->CalcCircumcircle();
}

Triangle::Triangle(const Vec &A, const Vec &B,const Vec &C, const bool TempVertexFlg, const bool _visibleFlg): 
					a(A), b(B), c(C), tempVertexFlg(TempVertexFlg), visibleFlg(_visibleFlg)
{
	this->CalcCircumcircle();
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
	return  this->CalcDistanceFromCenterSQ(point) + EPS < this->circumcircle.rSQ;
}


void	calcMidPoint(const Vec &a, const Vec &b, double midPoint[2])
{
	midPoint[0]	= (a.x + b.x) / 2.0;
	midPoint[1]	= (a.y + b.y) / 2.0;
}

void	calcLinearEquation(const Vec &a, const Vec &b, double	coeff[2], double &constNum)
{
	double	midPoint[2];

	calcMidPoint(a, b, midPoint);
	double	xCoeff = b.y - a.y;
	double	yCoeff = -(b.x - a.x);

	coeff[0] = -yCoeff;
	coeff[1] = xCoeff;

	constNum = coeff[0] * midPoint[0] + coeff[1] * midPoint[1];
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
		// std::cout << "det is 0" << std::endl;
		this->circumcircle.center.x = 0.0;
		this->circumcircle.center.y = 0.0;
		this->circumcircle.rSQ = 0.0;
		return;
	}

	this->circumcircle.center.x = (abConstNum * acCoeff[1] - acConstNum * abCoeff[1]) / det;
	this->circumcircle.center.y = (abCoeff[0] * acConstNum - acCoeff[0] * abConstNum) / det;
	this->circumcircle.rSQ = this->a.MagnitudeSQ2d(this->circumcircle.center);
	this->circumcircle.r   = sqrt(this->circumcircle.rSQ);
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
	this->n /= this->n.Magnitude3d();
	if (this->n.z < 0.0)
	{
		this->n *= -1.0;
	}
	this->g = (this->a + this->b + this->c) / 3.0;
}

double	Triangle::FindZ(const double pX, const double pY)
{
	return this->a.z - (this->n.x * (pX - this->a.x) + this->n.y * (pY - this->a.y)) / this->n.z;
}

void	Triangle::DrawFrame(const double midHeight, const Vec &halfMapSize)
{
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawVertex(move_vec_to_map_center(this->b, halfMapSize, midHeight));
	drawVertex(move_vec_to_map_center(this->a, halfMapSize, midHeight));
	drawVertex(move_vec_to_map_center(this->c, halfMapSize, midHeight));
	glEnd();
}

void	Triangle::DrawNormalVector(const double midHeight, const Vec &halfMapSize)
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawVertex(move_vec_to_map_center(this->g, halfMapSize, midHeight));
	drawVertex(move_vec_to_map_center(this->g + this->n * 10, halfMapSize, midHeight));
	glEnd();
}

void	Triangle::DrawTriangle(const int64_t	maxHeight,
							const int64_t	minHeight,
							const double	midHeight,
							const Vec &halfMapSize,
							const bool	lineFlg)
{
	if (!this->visibleFlg)
	{
		this->DrawFrame(midHeight, halfMapSize);
		return ;
	}
	if (lineFlg || !this->visibleFlg)
	{
		glBegin(GL_LINE_LOOP);
	}
	else
	{
		glShadeModel(GL_SMOOTH);
	}

	glBegin(GL_TRIANGLES);

	line_gradation(maxHeight, minHeight, midHeight, this->a.z);
	drawVertex(move_vec_to_map_center(this->a, halfMapSize, midHeight));
	line_gradation(maxHeight, minHeight, midHeight, this->b.z);
	drawVertex(move_vec_to_map_center(this->b, halfMapSize, midHeight));
	line_gradation(maxHeight, minHeight, midHeight, this->c.z);
	drawVertex(move_vec_to_map_center(this->c, halfMapSize, midHeight));

	glEnd();
}

void	Triangle::MoveVertexToMapCenter(const Vec &halfMapSize, const double midHeight)
{
	move_vec_to_map_center(this->a, halfMapSize, midHeight);
	move_vec_to_map_center(this->b, halfMapSize, midHeight);
	move_vec_to_map_center(this->c, halfMapSize, midHeight);
}

bool	Triangle::operator==(const Triangle &triangle) const
{
	return (this->a == triangle.a) && 
		   (this->b == triangle.b) &&
		   (this->c == triangle.c) &&
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
		this->g = triangle.g;
		this->circumcircle = triangle.circumcircle;
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
