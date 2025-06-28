#include "../includes/TerrestrialSimulation.hpp"
#include "../includes/ReadMapData.hpp"
#include "../includes/Graphic.hpp"
#include "../includes/Print.hpp"
#include "../includes/DelaunayTriangulation.hpp"
#include "../includes/Utils.hpp"
#include "../includes/MPS.hpp"
#include "../includes/Quadrangle.hpp"

TS::TS()
{

}

TS::~TS()
{

}

bool	TS::_ReadMapData(const std::string &fileName)
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

void	TS::_DrawSimulation(const int argc, const char** argv, 
							const std::deque<Triangle> &ts)
{
	Graphic g = Graphic(argc, argv, SCREEN_SIZE_X, SCREEN_SIZE_Y,
						ts, this->_mapSize, this->_maxHeight, this->_minHeight);
	g.GraphicLoop();
}

bool	TS::SimulationStart(const int argc, const char** argv, 
							const std::string &fileName)
{
	if (this->_ReadMapData(fileName) == false)
	{
		return false;
	}

	DT	dT(this->_specificPoints, this->_mapSize,
		   this->_maxHeight, this->_minHeight);
	std::deque<Triangle>	ts = dT.Calculation();
	this->_DrawSimulation(argc, argv, ts);

	return true;
}

void	TS::PrintSpecificPoints()
{
	for (size_t i = 0; i < this->_specificPoints.size(); ++i)
	{
		Print::OutWords(this->_specificPoints[i]);
	}
}
