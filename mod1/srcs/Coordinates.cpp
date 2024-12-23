#include "../includes/Coordinates.hpp"

std::ostream &operator<<(std::ostream &ostrm, const Coordinates &coordinates)
{
	return ostrm << '(' << coordinates.x << ", " 
						<< coordinates.y << ", "
						<< coordinates.z << ')' 
						<< std::endl;
}