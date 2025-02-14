
#include "../includes/ThreeDSpline.hpp"

ThreeDSpline::ThreeDSpline(const std::deque<Vec> &specificPoints, 
						   const uint32_t mapSize[2]): _specificPonints(specificPoints)
{
	this->CreateSpecificPonints(mapSize);
}

void	ThreeDSpline::CreateSpecificPonints(const uint32_t mapSize[2])
{
	for (uint32_t x = 0; x < mapSize[X]; ++x)
	{
		if (x == 0 || x == mapSize[X] - 1)
		{
			for (uint32_t y = 0; y < mapSize[Y]; ++y)
			{
				this->_specificPonints.push_back(Vec(x, y, 0));
			}
		}
		else
		{
			this->_specificPonints.push_back(Vec(x, 0, 0));
			this->_specificPonints.push_back(Vec(x, mapSize[Y] - 1, 0));
		}
	}

	for (size_t	i = 0; i < this->_specificPonints.size(); ++i)
	{
		std::cout << this->_specificPonints[i] << std::endl;
	}
}

