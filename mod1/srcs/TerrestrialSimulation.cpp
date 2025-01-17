#include "../includes/TerrestrialSimulation.hpp"
#include "../includes/ParseModFile.hpp"
#include "../includes/Graphic.hpp"
#include "../includes/RenderingAlgorithm.hpp"
#include "../includes/Print.hpp"

TerrestrialSimulation::TerrestrialSimulation()
{
	
}

TerrestrialSimulation::~TerrestrialSimulation()
{

}

bool	TerrestrialSimulation::SimulationStart(int argc, 
											   char** argv, 
											   const std::string &fileName)
{
	if (ParseModFile(fileName, this->_vecDeque) == false)
	{
		return false;
	}
	this->DecideMapSize();
	Graphic g = Graphic(argc, argv);
	g.GraphicLoop(RenderingAlgorithm);

	this->PrintVecDeque();
	return true;
}

void	TerrestrialSimulation::DecideMapSize()
{
	int32_t	xMin = 0;
	int32_t xMax = 0;
	int32_t	yMin = 0;
	int32_t	yMax = 0;

	for (size_t	i = 0; i < this->_vecDeque.size(); ++i)
	{
		if (this->_vecDeque[i].x < xMin)
		{
			xMin = this->_vecDeque[i].x;
		}
		else if (xMax < this->_vecDeque[i].x)
		{
			xMax = this->_vecDeque[i].x;
		}

		if (this->_vecDeque[i].y < yMin)
		{
			yMin = this->_vecDeque[i].y;
		}
		else if (yMax < this->_vecDeque[i].y)
		{
			yMax = this->_vecDeque[i].y;
		}
	}

	this->_mapSize[0] = xMax + xMin;
	this->_mapSize[1] = yMax + yMin;
}

void	TerrestrialSimulation::PrintVecDeque()
{
	for (size_t i = 0; i < this->_vecDeque.size(); ++i)
	{
		std::cout << this->_vecDeque[i];
	}
}