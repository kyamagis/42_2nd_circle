#include "../includes/TerrestrialSimulation.hpp"
#include "../includes/ParseModFile.hpp"
#include "../includes/Graphic.hpp"
#include "../includes/RenderingAlgorithm.hpp"
#include "../includes/Print.hpp"

#define X 0
#define Y 1
#define Z 0
#define IS_SPECIFIED 1
#define TRUE 1
#define FALSE 0

#include <string.h>

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
	this->NewThreeDCoordinates();
	Graphic g = Graphic(argc, argv);
	g.GraphicLoop(RenderingAlgorithm);

	this->PrintSpecificPoints();
	return true;
}

void	TerrestrialSimulation::NewThreeDCoordinates()
{
	// this->_threeDCoordinates = new int32_t**[this->_mapSize[X]];
	// for (uint32_t x = 0; x < this->_mapSize[X]; ++x)
	// {
	// 	this->_threeDCoordinates[x] = new int32_t*[this->_mapSize[Y]];
	// 	for (uint32_t y = 0; y < this->_mapSize[Y]; ++y)
	// 	{
	// 		this->_threeDCoordinates[x][y] = new int32_t[2];

	// 		memset(this->_threeDCoordinates[x][y], 0, sizeof(this->_threeDCoordinates[x][y]));
	// 		if (x == 0 || y == 0 || 
	// 			x == this->_mapSize[X] - 1 || 
	// 			y == this->_mapSize[Y] - 1)
	// 		{
	// 			this->_threeDCoordinates[x][y][IS_SPECIFIED] = TRUE;
	// 		}
	// 	}
	// }

	// for (uint32_t i = 0; i < this->_specificPoints.size(); ++i)
	// {
	// 	this->_threeDCoordinates[this->_specificPoints[i].x][this->_specificPoints[i].y][Z]
	// 	 = this->_specificPoints[i].z;
	// 	this->_threeDCoordinates[this->_specificPoints[i].x][this->_specificPoints[i].y][IS_SPECIFIED]
	// 	 = TRUE;
	// }

	// this->_specificPoints.clear();
}

void	TerrestrialSimulation::DecideMapSize()
{
	int32_t	xMin = 100;
	int32_t xMax = 100;
	int32_t	yMin = 0;
	int32_t	yMax = 0;

	for (size_t	i = 0; i < this->_specificPoints.size(); ++i)
	{
		if (this->_specificPoints[i].x < xMin)
		{
			xMin = this->_specificPoints[i].x;
		}
		else if (xMax < this->_specificPoints[i].x)
		{
			xMax = this->_specificPoints[i].x;
		}

		if (this->_specificPoints[i].y < yMin)
		{
			yMin = this->_specificPoints[i].y;
		}
		else if (yMax < this->_specificPoints[i].y)
		{
			yMax = this->_specificPoints[i].y;
		}
	}

	this->_mapSize[X] = xMax + xMin;
	this->_mapSize[Y] = yMax + yMin;
}

void	TerrestrialSimulation::PrintSpecificPoints()
{
	for (size_t i = 0; i < this->_specificPoints.size(); ++i)
	{
		std::cout << this->_specificPoints[i];
	}
}