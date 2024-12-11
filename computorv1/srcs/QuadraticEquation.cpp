 #include "../includes/QuadraticEquation.hpp"

 #include <iostream>
 #include <ctype.h>
 #include <math.h>
 #include <string.h>
 #include <iomanip> 
 
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
	double	minusB = -1.0 * this->equation[1];
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
		double	rootD = sqrt(d);
		double	plus  = vertex + rootD / twoA;
		double	minus = vertex - rootD / twoA;

		if (plus < minus)
		{
			double tmp = plus;
			plus = minus;
			minus = tmp;
		}

		oss << std::defaultfloat << plus << "\n" << minus;
		this->PrintAns(solutionStr + "positive, the two solutions are: \n" + oss.str());
	}
	else if (d < 0.0)
	{
		this->PrintAns(solutionStr + "negative, solution is no real numer");
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
	this->PrintAns("I can't solve constant equation .");
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
		absCoefficient = abs(this->equation[i]);
		
		if (this->equation[i] < 0)
		{
			ansStr += " - ";
		}
		else if (0 < this->equation[i])
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
		// ansStr += std::to_string(absCoefficient) + x + std::to_string(i);
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

	// std::cout << &quadraticEquationStr[i] << std::endl;
	for(; isblank(quadraticEquationStr[i]); ++i);
	if (!this->StorSign(quadraticEquationStr, i, sign))
	{
		return false;
	}
	// std::cout << &quadraticEquationStr[i] << std::endl;
	for(; isblank(quadraticEquationStr[i]); ++i);
	if (!this->ToDouble(quadraticEquationStr, i, coefficient))
	{
		return false;
	}
	// std::cout << &quadraticEquationStr[i] << std::endl;
	for(; isblank(quadraticEquationStr[i]); ++i);
	if (!this->IsAsterisk(quadraticEquationStr, i))
	{
		return false;
	}
	// std::cout << &quadraticEquationStr[i] << std::endl;
	for(; isblank(quadraticEquationStr[i]); ++i);
	if (!this->StoreCoefficient(quadraticEquationStr, i, sign, coefficient))
	{
		return false;
	}
	// std::cout << &quadraticEquationStr[i] << std::endl;
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
		else if (this->sideFlg = RightSide)
		{
			std::cerr << "err: double equal:" << &quadraticEquationStr[i] << std::endl;
			return false;
		}
	}
	return true;
}