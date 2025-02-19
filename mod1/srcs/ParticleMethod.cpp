#include "../includes/ParticleMethod.hpp"
#include "../includes/MathUtils.hpp"

#define NUM_OF_PARTICLES 100
#define	D 3 // dimension number
#define	GRADIENT  0
#define LAPLACIAN 1

PM::PM()
{
	this->ps.resize(100);
}

PM::~PM()
{

}

double	PM::W(const size_t i, const size_t oneself, bool gradientFlg) // weight
{
	double	rSQ = magnitude(this->ps[i].center.x, this->ps[i].center.y, 
						    this->ps[oneself].center.x, this->ps[oneself].center.y);

	if (gradientFlg == GRADIENT)
	{
		return E_RADIUS / rSQ - rSQ / E_RADIUS;
	}

	return E_RADIUS / rSQ + rSQ / E_RADIUS - 2;
}

void	PM::PressureGradientTerm(Vec &p, const size_t oneself)
{

}

void	PM::ViscosityTerm(Vec &vi, const size_t oneself)
{
	double	kv; // Kinematic viscosity coefficient
	double	lambda;
	double	n0;

	for (size_t	i = 0; i < this->ps.size(); ++i)
	{
		if (i == oneself)
		{
			continue;
		}
		vi += (this->ps[i].velocity - this->ps[oneself].velocity) * 
			   this->W(i, oneself, GRADIENT);
	}

	vi *= kv * (2 * D / lambda * n0); 
}

void	PM::NavierStokesEquations(const	Vec &g)
{
	//PressureGradientTerm + ViscosityTerm + g
}

// PM::PM(const PM &PM)
// {
// 	*this = PM;
// }

// bool	PM::operator==(const PM &PM) const
// {
// 	return (this->x == PM.x) && 
// 		   (this->y == PM.y) &&
// 		   (this->r == PM.r);
// }

// PM&	PM::operator=(const PM &PM)
// {
// 	if (this != &PM)
// 	{
// 		this->x = PM.x;
// 		this->y = PM.y;
// 		this->r = PM.r;
// 	}
// 	return *this;
// }

// std::ostream &operator<<(std::ostream &ostrm, const PM &PM)
// {
// 	return ostrm << '(' << PM.x << ", " 
// 						<< PM.y << ", "
// 						<< PM.r << ')' 
// 						<< std::endl;
// }