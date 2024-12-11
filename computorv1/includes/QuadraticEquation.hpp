#ifndef QUADRATICEUATION_HPP
# define QUADRATICEUATION_HPP

#include <string>
#include <stdio.h>

enum	EquationSide
{
	LeftSide = 1,
	RightSide = -1
};

class QuadraticEquation
{
	private:
		double			equation[3];
		EquationSide	sideFlg = LeftSide;

		QuadraticEquation(const QuadraticEquation &_QuadraticEquation);
		QuadraticEquation&	operator=(const QuadraticEquation &_QuadraticEquation);

	public:
		QuadraticEquation();
		~QuadraticEquation();
		int		Compute(const char *quadraticEquationStr);
		bool	StoresCoefficients(const char *quadraticEquationStr, size_t &i);
		bool	StorSign(const char *quadraticEquationStr, size_t &i, double &sign);
		bool	ToDouble(const char *quadraticEquationStr, size_t &i, double &coefficient);
		bool	IsAsterisk(const char *quadraticEquationStr, size_t &i);
		bool	StoreCoefficient(const char *quadraticEquationStr, size_t &i, double sign, double coefficient);
		bool	DetectEqual(const char *quadraticEquationStr, size_t &i);
		void	Solution(void);
		void	QuadraticEquationSolving(void);
		double	Discriminant(void);
		void	LinearEquationSolving(void);
		void	ConstantEquationSolving(void);
		void	PrintAns(std::string solutionStr);
};

#endif