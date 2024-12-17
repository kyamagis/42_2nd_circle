#include "../includes/QuadraticEquation.hpp"
#include "../includes/Mysqrt.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Solutions.hpp"

#include <iostream>
#include <ctype.h>
#include <string.h>
#include <iomanip>

#ifdef DEBUG
	#include <math.h>
#endif
 
QuadraticEquation::QuadraticEquation()
{
	for(size_t  i = 0; i < 3; ++i)
	{
		this->equation[i]  = 0.0;
	}
}

QuadraticEquation::~QuadraticEquation()
{

}

int	QuadraticEquation::Compute(const char *quadraticEquationStr)
{
	size_t	len = strlen(quadraticEquationStr);

	for(size_t	i = 0; i < len;)
	{
		if (!this->StoresCoefficients(quadraticEquationStr, i))
		{
			return 1;
		}
	}
	if (this->sideFlg == LeftSide)
	{
		return 1;
	}
	this->Solution();
	return 0;
}

void	QuadraticEquation::Solution(void)
{
	if (this->equation[2] == 0.0)
	{
		// 0次方程式の場合
		if (this->equation[1] == 0.0)
		{
			this->ConstantEquationSolving();
		}
		// 1次方程式の場合
		else if (this->equation[1] != 0.0)
		{
			this->LinearEquationSolving();
		}
		return ;
	}
	// 2次方程式の場合
	this->QuadraticEquationSolving();
}

void	QuadraticEquation::QuadraticEquationSolving(void)
{
	double	d      = this->Discriminant();
	double	minusB = this->equation[1];
	if (this->equation[1] != 0.0)
	{
		minusB *= -1.0;
	}
	double	twoA   = 2.0 * this->equation[2];
	double	vertex = minusB / twoA;
	std::string	solutionStr = "Discriminant is strictly ";
	std::ostringstream oss;

	if (d == 0.0)
	{
		oss << std::defaultfloat << vertex;
		this->PrintAns(solutionStr + "0, the solution is: \n" + oss.str());
	}
	else if (0.0 < d)
	{
		double					rootD = SqrtPureNewtonMethod(d, MAX_ACCURACY);
		std::array<double, 2>	solutionArray = QuadraticFormula(rootD, twoA, vertex);

		oss << std::defaultfloat << solutionArray[0] << "\n" << solutionArray[1];
	
		this->PrintAns(solutionStr + "positive, the two real solutions are: \n" + oss.str());

		#ifdef DEBUG
			rootD = sqrt(d);
			solutionArray = QuadraticFormula(rootD, twoA, vertex);
			oss.str("");
			oss << std::defaultfloat << "sqrt solutions: \n"
									 << solutionArray[0] << "\n" 
									 << solutionArray[1];
			std::cout << oss.str() << std::endl;
		#endif
	}
	else if (d < 0.0)
	{
		d *= -1.0;
		double	rootD = SqrtPureNewtonMethod(d, MAX_ACCURACY);
		double	halfWidth = rootD / twoA;

		oss << std::defaultfloat << vertex << " + " << halfWidth << " * i " << "\n"
								 << vertex << " - " << halfWidth << " * i ";							;

		this->PrintAns(solutionStr + "negative, the two complex solutions are: \n" + oss.str());

		#ifdef DEBUG
			rootD = sqrt(d);
			halfWidth = rootD / twoA;
			oss.str("");
			oss << std::defaultfloat << "sqrt solutions: \n" 
									 << vertex << " + " << halfWidth << " * i " << "\n"
									 << vertex << " - " << halfWidth << " * i ";
			std::cout << oss.str() << std::endl;
		#endif
	}
}

double	QuadraticEquation::Discriminant(void)
{
	return (this->equation[1] * this->equation[1]) - 
			(4.0 * this->equation[2] * this->equation[0]);
}

void	QuadraticEquation::LinearEquationSolving(void)
{
	double	ans = this->equation[0] / this->equation[1];
	ans *= -1.0;
	std::ostringstream oss;
	oss << std::defaultfloat << ans;
	this->PrintAns("The solution degree: \n" + oss.str());
}

void	QuadraticEquation::ConstantEquationSolving(void)
{
	if (this->equation[0] == 0.0)
	{
		this->PrintAns("The solution degree: \n0 = 0");
		return;
	}
	this->PrintAns("The wrong equation.");
}

void	QuadraticEquation::PrintAns(std::string solutionStr)
{
	std::string	ansStr = "Reduced form:";
	std::string	x = " * X^";
	bool		headFlg = true;
	double		absCoefficient = 0.0;
	int			polynomialDegree = 0;

	for (size_t	i = 0; i < 3; ++i)
	{
		if (this->equation[i] == 0.0)
		{
			continue;
		}
		polynomialDegree = i;
		absCoefficient = MyAbs(this->equation[i]);
		
		if (this->equation[i] < 0.0)
		{
			if (headFlg == true)
			{
				headFlg = false;
				ansStr += " -";
			}
			else
			{
				ansStr += " - ";
			}
		}
		else if (0.0 <= this->equation[i])
		{
			if (headFlg == true)
			{
				headFlg = false;
				ansStr += ' ';
			}
			else
			{
				ansStr += " + ";
			}
		}
		else
		{
			std::cout << this->equation[i] << std::endl;
		}
		std::ostringstream oss;
		oss << std::defaultfloat << absCoefficient;
		ansStr += oss.str() + x + std::to_string(i);
	}
	ansStr += " = 0\nPolynomial degree: " + 
			  std::to_string(polynomialDegree) + '\n';
	std::cout << std::defaultfloat;
	std::cout << ansStr << solutionStr << std::endl;
}

bool	QuadraticEquation::StoresCoefficients(const char *quadraticEquationStr, size_t &i)
{
	double	sign = 1.0;
	double	coefficient = 0.0;

	//std::istringstream iss;
	for(; isblank(quadraticEquationStr[i]); ++i);
	if (!this->StorSign(quadraticEquationStr, i, sign))
	{
		return false;
	}
	for(; isblank(quadraticEquationStr[i]); ++i);
	if (!this->ToDouble(quadraticEquationStr, i, coefficient))
	{
		return false;
	}
	for(; isblank(quadraticEquationStr[i]); ++i);
	if (!this->IsAsterisk(quadraticEquationStr, i))
	{
		return false;
	}
	for(; isblank(quadraticEquationStr[i]); ++i);
	if (!this->StoreCoefficient(quadraticEquationStr, i, sign, coefficient))
	{
		return false;
	}
	for(; isblank(quadraticEquationStr[i]); ++i);
	if (!this->DetectEqual(quadraticEquationStr, i))
	{
		return false;
	}

	return true;
}

// err: "+ a"
// err: "+"

bool	QuadraticEquation::StorSign(const char *quadraticEquationStr, size_t &i, double &sign)
{
	if (quadraticEquationStr[i] == '-')
	{
		sign = -1.0;
		++i;
		
		return (true);
	}
	else if (quadraticEquationStr[i] == '+')
	{
		++i;
		return (true);
	}
	else if (isdigit(quadraticEquationStr[i]))
	{
		return (true);
	}
	std::cerr << "sign err: " << &quadraticEquationStr[i] << std::endl;
	return (false);
}

bool	QuadraticEquation::ToDouble(const char *quadraticEquationStr, size_t &i, double &coefficient)
{
	std::string	numStr = "";
	size_t		start = i;

	// .12 の対策
	if (!isdigit(quadraticEquationStr[i]))
	{
		std::cerr << "err: coefficients contain non-numbers: " << &quadraticEquationStr[i] << std::endl;
		return false;
	}
	/* 
		for (; isdigit(quadraticEquationStr[i]); ++i) でエラー

	*/
	for (; isdigit(quadraticEquationStr[i]); ++i);
	if (quadraticEquationStr[i] == '.')
	{
		do
		{
			++i;
		} while (isdigit(quadraticEquationStr[i]));
	}
	if (isblank(quadraticEquationStr[i]) || quadraticEquationStr[i] == '*')
	{
		numStr = std::string(quadraticEquationStr + start, i - start);
		coefficient = std::stod(numStr);
		return true;
	}

	std::cerr << "err: coefficients contain non-numbers: " << &quadraticEquationStr[i] << std::endl;
	return false;
}

bool	QuadraticEquation::IsAsterisk(const char *quadraticEquationStr, size_t &i)
{
	if (quadraticEquationStr[i] == '*')
	{
		++i;
		return true;
	}
	std::cerr << "err: expect asterisk :" << &quadraticEquationStr[i] << std::endl;
	return false;
}

bool	QuadraticEquation::StoreCoefficient(const char *quadraticEquationStr, size_t &i, double sign, double coefficient)
{
	if (strlen(&quadraticEquationStr[i]) < 2 ||
		strncmp(&quadraticEquationStr[i], "X^", 2) != 0)
	{
		std::cerr << "err: expect X^ :" << &quadraticEquationStr[i] << std::endl;
		return false;
	}
	i += 2;
	int exponent = quadraticEquationStr[i] - '0';
	if (0 <= exponent && exponent <= 2)
	{
		this->equation[exponent] += this->sideFlg * sign * coefficient;
		i++;
		if (isblank(quadraticEquationStr[i])
			|| quadraticEquationStr[i] == '='
			|| quadraticEquationStr[i] == '-'
			|| quadraticEquationStr[i] == '+'
			|| quadraticEquationStr[i] == '\0')
		{
			return true;
		}
	}
	std::cerr << "err: exponent is only 0 to 2 :" << &quadraticEquationStr[i] << std::endl;
	return false;
}

bool	QuadraticEquation::DetectEqual(const char *quadraticEquationStr, size_t &i)
{
	if (quadraticEquationStr[i] == '=')
	{
		if (this->sideFlg == LeftSide)
		{
			this->sideFlg = RightSide;
			++i;
			return true;
		}
		else if (this->sideFlg == RightSide)
		{
			std::cerr << "err: double equal:" << &quadraticEquationStr[i] << std::endl;
			return false;
		}
	}
	return true;
}