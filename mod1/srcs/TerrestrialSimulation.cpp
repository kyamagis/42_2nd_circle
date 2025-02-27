#include "../includes/TerrestrialSimulation.hpp"
#include "../includes/ReadMapData.hpp"
#include "../includes/Graphic.hpp"
#include "../includes/Print.hpp"
#include "../includes/DelaunayTriangulation.hpp"
#include "../includes/Utils.hpp"
#include "../includes/MPS.hpp"

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

bool	TerrestrialSimulation::SimulationStart(int argc, 
											   char** argv, 
											   const std::string &fileName)
{
	if (this->_ReadMapData(fileName) == false)
	{
		return false;
	}

	DT	dT(this->_specificPoints, this->_mapSize, 
		   this->_maxHeight, this->_minHeight);
	std::deque<Triangle>	ts = dT.Calculation();


	// uint32_t cubeMapSize[3];
	// cubeMapSize[X] = 1;
	// cubeMapSize[Y] = 1;
	// cubeMapSize[Z] = 1;


	// Vec	a(0,0,0);
	// Vec	b(0,0,cubeMapSize[Z]);
	// Vec	c(cubeMapSize[X],0,cubeMapSize[Z]);
	// Vec	d(cubeMapSize[X],0,0);

	// Vec	e(0,cubeMapSize[Y],0);
	// Vec	f(0,cubeMapSize[Y],cubeMapSize[Z]);
	// Vec	g1(cubeMapSize[X],cubeMapSize[Y],cubeMapSize[Z]);
	// Vec	h(cubeMapSize[X],cubeMapSize[Y],0);

	// std::deque<Triangle> cube;
	// cube.push_back(Triangle(a,b,c, false));
	// cube.push_back(Triangle(c,d,a, false));

	// cube.push_back(Triangle(d,h,c, false));
	// cube.push_back(Triangle(h,g1,c, false));

	// cube.push_back(Triangle(b,f,g1, false));
	// cube.push_back(Triangle(g1,c,b, false));

	// MPS	mps();

	Graphic g = Graphic(argc, argv, SCREEN_SIZE_X, SCREEN_SIZE_Y);

	// g.InitGraphicData(cube, cubeMapSize, 1, 0);

	g.InitGraphicData(ts, this->_mapSize, this->_maxHeight, this->_minHeight);
	g.GraphicLoop();

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