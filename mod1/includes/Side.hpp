#ifndef SIDE_HPP
# define SIDE_HPP

#include <iostream>
#include <string>
#include "./Vec.hpp"

class Side
{
	private:

	public:
		Vec startPoint;
		Vec endPoint;

		Side();
		~Side();

		Side(Vec S, Vec E);

		bool	operator==(const Side &side) const;
		Side&	operator=(const Side &side);
};

std::ostream &operator<<(std::ostream &ostrm, const Side &Side);

#endif