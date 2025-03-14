#include "../includes/TerrestrialSimulation.hpp"
#include "../includes/ReadMapData.hpp"
#include "../includes/Graphic.hpp"
#include "../includes/Print.hpp"
#include "../includes/DelaunayTriangulation.hpp"
#include "../includes/Utils.hpp"
#include "../includes/MPS.hpp"
#include "../includes/Quadrangle.hpp"

TerrestrialSimulation::TerrestrialSimulation()
{

}

TerrestrialSimulation::~TerrestrialSimulation()
{

}

bool	TerrestrialSimulation::_ReadMapData(const std::string &fileName)
{
	RMD	rmd(fileName);

	if (rmd.ReadStart() == false)
	{
		return false;
	}
	this->_specificPoints = rmd.GetSpecificPoints();
	rmd.GetMapSize(this->_mapSize);
	rmd.GetMaxMinHeight(this->_maxHeight, this->_minHeight);

	return	true;
}

void	TerrestrialSimulation::_DrawSimulation(const int argc, 
												const char** argv, 
												const std::deque<Triangle> &ts)
{
	Graphic g = Graphic(argc, argv, SCREEN_SIZE_X, SCREEN_SIZE_Y);

	g.InitGraphicData(ts, this->_mapSize, this->_maxHeight, this->_minHeight);
	g.GraphicLoop();
}

bool	TerrestrialSimulation::SimulationStart(const int argc, 
											   const char** argv, 
											   const std::string &fileName)
{
	if (this->_ReadMapData(fileName) == false)
	{
		return false;
	}

	// DT	dT(this->_specificPoints, this->_mapSize, 
	// 	   this->_maxHeight, this->_minHeight);
	// std::deque<Triangle>	ts = dT.Calculation();

	std::deque<Triangle>	ts;

	this->_DrawSimulation(argc, argv, ts);

	// this->PrintSpecificPoints();
	return true;
}

void	TerrestrialSimulation::PrintSpecificPoints()
{
	for (size_t i = 0; i < this->_specificPoints.size(); ++i)
	{
		std::cout << this->_specificPoints[i];
	}
}
