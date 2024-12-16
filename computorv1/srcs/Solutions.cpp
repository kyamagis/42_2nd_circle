#include "../includes/QuadraticEquation.hpp"
#include "../includes/Mysqrt.hpp"
#include "../includes/Solutions.hpp"

#include <utility>

#ifdef DEBUG
	#include <math.h>
#endif

std::array<double, 2>	QuadraticFormula(double rootD, double twoA, double vertex)
{
	std::array<double, 2>	solutionArray;
	double					halfWidth = rootD / twoA;

	solutionArray[0] = vertex + halfWidth;
	solutionArray[1] = vertex - halfWidth;

	if (solutionArray[0] < solutionArray[1])
	{
		std::swap(solutionArray[0], solutionArray[1]);
	}

	return solutionArray;
}