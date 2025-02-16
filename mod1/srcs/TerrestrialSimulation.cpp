#include "../includes/TerrestrialSimulation.hpp"
#include "../includes/ParseModFile.hpp"
#include "../includes/Graphic.hpp"
// #include "../includes/RenderingAlgorithm.hpp"
#include "../includes/Print.hpp"
#include "../includes/DelaunayTriangulation.hpp"

#define X 0
#define Y 1

#define SCREEN_SIZE_X 1000
#define SCREEN_SIZE_Y 1000

#define SCALING 0.3


#include <string.h>
#include <GL/glut.h>
#include <cmath>
const float PI = 3.14159265358979323846;

typedef struct s_data
{
	std::deque<Triangle> triangles;
	uint32_t	mapSize[2];
} t_data;


static t_data g_data;

TerrestrialSimulation::TerrestrialSimulation(): _threeDCoordinates(NULL)
{

}

TerrestrialSimulation::~TerrestrialSimulation()
{

}

void	Vertex(const Vec &ver)
{
	double	shrinkageRatioX = 1.0 / g_data.mapSize[X] * 2;
	double	shrinkageRatioY = 1.0 / g_data.mapSize[Y] * 2;

	double	coordinateX = ver.x * shrinkageRatioX - 1.0;
	double	coordinateY =  - 1 * (ver.y * shrinkageRatioY - 1.0);

	glVertex2d(coordinateX * SCALING , coordinateY * SCALING);
	// std::cout <<ver.x * shrinkageRatioX - 1 << ", " << ver.y * shrinkageRatioY - 1 << std::endl;
}

void	drawTriangle(const Triangle &triangle)
{
	glBegin(GL_LINE_LOOP);
	glColor3d(1.0, 0.0, 0.0);
	Vertex(triangle.a);
	Vertex(triangle.b);
	Vertex(triangle.c);
	glEnd();
}

void drawCircle(double cx, double cy, double r, int num_segments) {
    glBegin(GL_LINE_LOOP); // 円を線で描く
	glColor3d(0.0, 1.0, 0.0);

	double	shrinkageRatioX = 1.0 / g_data.mapSize[X] * 2;
	double	shrinkageRatioY = 1.0 / g_data.mapSize[Y] * 2;

	cx = cx * shrinkageRatioX - 1.0;
	cy =  - 1 * (cy * shrinkageRatioY - 1.0);
	r *= shrinkageRatioX;

    for (int i = 0; i < num_segments; i++) {
        double theta = 2.0 * PI * double(i) / double(num_segments); // 角度
        double x = r * cos(theta); // X座標
        double y = r * sin(theta); // Y座標
        glVertex2d((cx + x) * SCALING, (cy + y) * SCALING);
    }
    glEnd();
}

void	RenderingAlgorithm()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	
	// glBegin(GL_TRIANGLES );

	// std::cout << g_data.mapSize[X] << std::endl;
	// std::cout << g_data.mapSize[Y] << std::endl;

	for (size_t	i = 0; i < g_data.triangles.size(); ++i)
	{
		drawTriangle(g_data.triangles[i]);
		// std::cout << g_data.triangles[i].circumcircle << sqrt(g_data.triangles[i].circumcircle.r) <<  std::endl;
		
		// drawCircle(g_data.triangles[i].circumcircle.x, 
		// 			g_data.triangles[i].circumcircle.y, 
		// 			sqrt(g_data.triangles[i].circumcircle.r), 
		// 			100);
	}

	// drawCircle(15000, 
	// 			12600, 
	// 			1000, 
	// 				100);
	
	// drawCircle(12600, 15000, 1000, 100);
	
	// Triangle t(Vec(45000, 0, 0), Vec(0, 45000, 0), Vec(12600, 15000, 3000), false);
	// drawCircle(t.circumcircle.x, 
	// 				t.circumcircle.y, 
	// 				sqrt(t.circumcircle.r), 
	// 				100);
	
	// glVertex2d(0.5, -0.5);
	// glVertex2d(0.5, 0.5);

	glFlush();
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

	DT	dT(this->_specificPoints, this->_mapSize);

	g_data.triangles = dT.Calculation();
	g_data.mapSize[X] = this->_mapSize[X];
	g_data.mapSize[Y] = this->_mapSize[Y];
	
	Graphic g = Graphic(argc, argv, SCREEN_SIZE_X, SCREEN_SIZE_Y);
	g.GraphicLoop(RenderingAlgorithm);

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

	this->_mapSize[X] = xMax + xMax / 2;
	this->_mapSize[Y] = yMax + yMax / 2;

	if (this->_mapSize[X] < this->_mapSize[Y])
	{
		this->_mapSize[X] = this->_mapSize[Y];
	}
	else
	{
		this->_mapSize[Y] = this->_mapSize[X];
	}
}

void	TerrestrialSimulation::PrintSpecificPoints()
{
	for (size_t i = 0; i < this->_specificPoints.size(); ++i)
	{
		std::cout << this->_specificPoints[i];
	}
}