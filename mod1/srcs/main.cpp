#include <omp.h>

#include "../includes/Print.hpp"
#include "../includes/TerrestrialSimulation.hpp"

#define DEFAULT_FILE_NAME "./resources/demo1.mod1"

int	main(int argc, char **argv)
{
	TS	tS;

	omp_set_num_threads(8);

	switch (argc)
	{
		case 1:
			if (tS.SimulationStart(argc, const_cast<const char**>(argv), DEFAULT_FILE_NAME) == false)
			{
				return EXIT_FAILURE;
			}
			return EXIT_SUCCESS;
		case 2:
			if (tS.SimulationStart(argc, const_cast<const char**>(argv), argv[1]) == false)
			{
				return EXIT_FAILURE;
			}
			return EXIT_SUCCESS;
		default :
			Print::Err("Arg is only 0 to 1");
			return EXIT_FAILURE;
	}
}
