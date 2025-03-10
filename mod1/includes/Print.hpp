#ifndef PRINT_HPP
# define PRINT_HPP

#include <iostream>
#include <string>

class Print
{
	public:

		template <typename T>
		static void OutWords(T word) 
		{
			std::cout << word << std::endl;
		}

		template <typename T, typename... Args>
		static bool OutWords(T first, Args... rest) 
		{
			std::cout << first << " ";
			OutWords(rest...);
			return true;
		}
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