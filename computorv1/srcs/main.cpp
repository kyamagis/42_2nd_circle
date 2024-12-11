#include <iostream>
#include "../includes/QuadraticEquation.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Argument must be one" << std::endl;
		return(1) ;
	}
	QuadraticEquation	qE;

	return qE.Compute(argv[1]);
}