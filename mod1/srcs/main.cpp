#include "../includes/Print.hpp"
#include "../includes/Coordinates.hpp"
#include "../includes/ParseModFile.hpp"

int	main(int argc, char **argv)
{
	switch (argc)
	{
		case 1:
			return EXIT_SUCCESS;
		case 2:
			{std::deque<Coordinates> posDeque;
			ParseModFile(argv[1], posDeque);
			for (size_t i = 0; i < posDeque.size(); ++i)
			{
				std::cout << posDeque[i];
			}
			return EXIT_SUCCESS;
			}
		default :
			Print::Err("Arg is only 0 to 1");
			return EXIT_FAILURE;
	}
}