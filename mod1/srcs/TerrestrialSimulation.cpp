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
// //////////////////////////////////////////////////////////////////////////////////////////

typedef struct s_testdata
{
	int64_t 				**map;
	double					mapSize[2];
	double					halfMapsize[2];
	std::deque<Triangle>	ts;
	std::deque<Triangle>	origenTs;
	std::deque<Quadrangle>	qs;
	MPS						*mps;

	size_t	numOfBuckets;
	double	maxHeight;
	int64_t	minHeight;
	double	midHeight;
	double	shrinkageRatioX;
	double	shrinkageRatioY;
	double	radX;
	double	radY;
	double	radZ;
	double	scaling;
	size_t	i;
	size_t	count;
	Vec		rotatedVertex;
	bool	circleFlg;
	int		gWindowID;

	unsigned char key;
} t_testdata;

static t_testdata g_testdata;

void	RenderingAlgorithm2(void)
{
	// std::cout << "RenderingAlgorithm2 " << g_testdata.radY++ << std::endl;
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // カラー & Zバッファーをクリア
	
	for (size_t	i = 0; i < g_testdata.qs.size(); ++i)
	{
		g_testdata.qs[i].DrawQuad();
	}
	glFlush();
}

std::deque<Triangle>	TerrestrialSimulation::_CalcWeight(const int argc, const char** argv)
{
	this->_mapSize[X] = 4.0 * BUCKET_LENGTH + 1;
	this->_mapSize[Y] = 4.0 * BUCKET_LENGTH + 1;
	this->_mapSize[Z] = 4.0 * BUCKET_LENGTH + 1;

	double doubleMapSize[3];

	doubleMapSize[X] = 4.0 * BUCKET_LENGTH;
	doubleMapSize[Y] = 4.0 * BUCKET_LENGTH;
	doubleMapSize[Z] = 4.0 * BUCKET_LENGTH;

	

	const Vec	a(0,0,0);
	const Vec	b(0,0,doubleMapSize[Z]);
	const Vec	c(doubleMapSize[X],0,doubleMapSize[Z]);
	const Vec	d(doubleMapSize[X],0,0);
	

	Vec	e(0,doubleMapSize[Y],0);
	Vec	f(0,doubleMapSize[Y],doubleMapSize[Z]);
	Vec	g(doubleMapSize[X],doubleMapSize[Y],doubleMapSize[Z]);
	Vec	h(doubleMapSize[X],doubleMapSize[Y],0);

	std::deque<Triangle> cube;
	// cube.push_back(Triangle(a,b,c, false));
	// cube.push_back(Triangle(c,d,a, false));

	// cube.push_back(Triangle(d,h,c, false));
	// cube.push_back(Triangle(h,g,c, false));

	// cube.push_back(Triangle(b,f,g, false));
	// cube.push_back(Triangle(g,c,b, false));

	// cube.push_back(Triangle(a,e,b, false));
	// cube.push_back(Triangle(e,f,b, false));

	g_testdata.qs.push_back(Quadrangle(a,b,c,d));
	g_testdata.qs.push_back(Quadrangle(f,g,c,b));

	g_testdata.mapSize[X] = doubleMapSize[X];
	g_testdata.mapSize[Y] = doubleMapSize[Y];
	g_testdata.maxHeight =  doubleMapSize[Z];
	g_testdata.scaling = SCALING;

	Graphic gr = Graphic(argc, argv, SCREEN_SIZE_X, SCREEN_SIZE_Y);

	gr.InitGraphicData(cube, this->_mapSize, this->_mapSize[Z], 0);
	// gr.GraphicLoop(RenderingAlgorithm2);
	glutDisplayFunc(RenderingAlgorithm2);
	glutMainLoop();

	return cube;
}
// //////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////

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

	// std::deque<Triangle> cube = this->_CalcWeight(argc, argv);
	DT	dT(this->_specificPoints, this->_mapSize, 
		   this->_maxHeight, this->_minHeight);
	std::deque<Triangle>	ts = dT.Calculation();

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
