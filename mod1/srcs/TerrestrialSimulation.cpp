#include "../includes/TerrestrialSimulation.hpp"
#include "../includes/ParseModFile.hpp"
#include "../includes/Graphic.hpp"
#include "../includes/Print.hpp"
#include "../includes/DelaunayTriangulation.hpp"
#include "../includes/MathUtils.hpp"

#define SCREEN_SIZE_X 1000
#define SCREEN_SIZE_Y 1000

TerrestrialSimulation::TerrestrialSimulation(): _threeDCoordinates(NULL)
{

}

TerrestrialSimulation::~TerrestrialSimulation()
{

}

bool	TerrestrialSimulation::SimulationStart(int argc, 
											   char** argv, 
											   const std::string &fileName)
{
	if (ParseModFile(fileName, this->_specificPoints) == false)
	{
		return false;
	}
	this->DecideMapSize();
	// std::cout << maxHeight << std::endl;
	
	// int64_t **map = new int64_t*[this->_mapSize[X]];

	// for (size_t	i = 0; i < this->_mapSize[X]; ++i)
	// {
	// 	map[i] = new int64_t[this->_mapSize[Y]];
	// }

	// dT.Calculation(map);

	// g_data.map = map;
	
	int64_t	maxHeight;
	int64_t	minHeight;
	DT	dT(this->_specificPoints, this->_mapSize);
	std::deque<Triangle>	ts = dT.Calculation(maxHeight, minHeight);

	Graphic g = Graphic(argc, argv, SCREEN_SIZE_X, SCREEN_SIZE_Y);

	g.InitGraphicData(ts, this->_mapSize, maxHeight, minHeight);
	g.GraphicLoop();

	// this->PrintSpecificPoints();
	return true;
}

void	TerrestrialSimulation::DecideMapSize()
{
	// int32_t	xMin = 100;
	int32_t xMax = 0;
	// int32_t	yMin = 100;
	int32_t	yMax = 0;

	for (size_t	i = 0; i < this->_specificPoints.size(); ++i)
	{
		// if (this->_specificPoints[i].x < xMin)
		// {
		// 	xMin = this->_specificPoints[i].x;
		// }
		// else 
		
		if (xMax < this->_specificPoints[i].x)
		{
			xMax = this->_specificPoints[i].x;
		}

		// if (this->_specificPoints[i].y < yMin)
		// {
		// 	yMin = this->_specificPoints[i].y;
		// }
		// else 
		if (yMax < this->_specificPoints[i].y)
		{
			yMax = this->_specificPoints[i].y;
		}
	}

	this->_mapSize[X] = xMax + 1 + xMax / 4;
	this->_mapSize[Y] = yMax + 1 + yMax / 4;

	if (this->_mapSize[X] < this->_mapSize[Y])
	{
		assign_power_of_two(this->_mapSize[X], this->_mapSize[Y]);
	}
	else
	{
		assign_power_of_two(this->_mapSize[Y], this->_mapSize[X]);
	}
}

void	TerrestrialSimulation::PrintSpecificPoints()
{
	for (size_t i = 0; i < this->_specificPoints.size(); ++i)
	{
		std::cout << this->_specificPoints[i];
	}
}