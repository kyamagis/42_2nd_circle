#ifndef PRINT_HPP
# define PRINT_HPP

#include <iostream>
#include <string>

class Print
{
	public:
		static bool	Out(const std::string &str)
		{
			std::cout << "\x1b[34m"<< str << "\033[m" << std::endl;
			return true;
		}

		static bool	Err(const std::string &str)
		{
			std::cerr << "\x1b[33m" << str << "\033[m" << std::endl;
			return false;
		}
};


#endif