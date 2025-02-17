#include "../includes/TerrestrialSimulation.hpp"
#include "../includes/ParseModFile.hpp"
#include "../includes/Graphic.hpp"
// #include "../includes/RenderingAlgorithm.hpp"
#include "../includes/Print.hpp"
#include "../includes/DelaunayTriangulation.hpp"
#include "../includes/MathUtils.hpp"

#define X 0
#define Y 1

#define SCREEN_SIZE_X 1000
#define SCREEN_SIZE_Y 1000

#define SCALING 0.3


#include <string.h>
#include <GL/glut.h>
#include <cmath>

typedef struct s_data
{
	int64_t 				**map;
	uint32_t				mapSize[2];
	std::deque<Triangle>	t;
	int64_t	maxHeight;
	int64_t	minHeight;
	double	radX;
	double	radY;
	double	radZ;
	double	scaling;
	Vec		rotatedVertex;
	int		gWindowID;
} t_data;


static t_data g_data;

TerrestrialSimulation::TerrestrialSimulation(): _threeDCoordinates(NULL)
{

}

TerrestrialSimulation::~TerrestrialSimulation()
{

}

void	Vertex(Vec vertex)
{

	z_rotation(vertex, g_data.radZ, vertex.x, vertex.y);
	x_rotation(vertex, g_data.radX, vertex.y, vertex.z);
	y_rotation(vertex, g_data.radY, vertex.x, vertex.z);

	double	shrinkageRatioX = 1.0 / g_data.mapSize[X] * 2;
	double	shrinkageRatioY = 1.0 / g_data.mapSize[Y] * 2;

	double	coordinateX = vertex.x * shrinkageRatioX - 1.0;
	double	coordinateY =  - 1 * (vertex.y * shrinkageRatioY - 1.0);

	glVertex2f(coordinateX * g_data.scaling , coordinateY * g_data.scaling);
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

	double	shrinkageRatioX = 1.0 / g_data.mapSize[X] * 2.0;
	double	shrinkageRatioY = 1.0 / g_data.mapSize[Y] * 2.0;

	cx = cx * shrinkageRatioX - 1.0;
	cy =  - 1 * (cy * shrinkageRatioY - 1.0);
	r = sqrt(r);
	r *= shrinkageRatioX;

    for (int i = 0; i < num_segments; i++) {
        double theta = 2.0 * M_PI * double(i) / double(num_segments); // 角度
        double x = r * cos(theta); // X座標
        double y = r * sin(theta); // Y座標
        glVertex2d((cx + x) * g_data.scaling, (cy + y) * g_data.scaling);
    }
    glEnd();
}

void drawCircle(const Triangle &t, int num_segments) {
    glBegin(GL_LINE_LOOP); // 円を線で描く
	glColor3d(0.0, 1.0, 0.0);

	double	shrinkageRatioX = 1.0 / g_data.mapSize[X] * 2.0;
	double	shrinkageRatioY = 1.0 / g_data.mapSize[Y] * 2.0;

	double	cx = double(t.circumcircle.x) * shrinkageRatioX - 1.0;
	double	cy = - 1.0 * (double(t.circumcircle.y) * shrinkageRatioY - 1.0);
	double	r = sqrt(t.circumcircle.r) * shrinkageRatioX;

	for (int i = 0; i < num_segments; i++) {
        double theta = 2.0 * M_PI * double(i) / double(num_segments); // 角度
        double x = r * cos(theta); // X座標
        double y = r * sin(theta); // Y座標
        glVertex2d((cx + x) * g_data.scaling, (cy + y) * g_data.scaling);
    }
    glEnd();
}

void	RenderingAlgorithm()
{
	glClear(GL_COLOR_BUFFER_BIT);
	// glColor3d(1.0, 0.0, 0.0);
	// glVertex2d(-0.9, -0.9);
	// glVertex2d(0.9, -0.9);
	// glVertex2d(0.9, 0.9);
	// glVertex2d(-0.9, 0.9);
	// glEnd();
	
	// glPointSize(5.0f);
	// glBegin(GL_POINTS);
	// for (size_t	x = 0; x < g_data.mapSize[X]; ++x)
	// {
	// 	for (size_t	y = 0; y < g_data.mapSize[Y]; ++y)
	// 	{
	// 		glColor3f(1.0, 0.0, 0.0);
	// 		// glVertex2d(double(x) / 10 * 2.0 - 1.0, double(y) / 10 * 2.0 - 1.0);
	// 		Vertex(Vec(g_data.mapSize[x], g_data.mapSize[y], 0));
	// 	}

	// 	std::cout << x << std::endl;
	// }
	
	
	for (size_t	i = 0; i < g_data.t.size(); ++i)
	{
		drawTriangle(g_data.t[i]);
		// drawCircle(g_data.t[i].circumcircle.x, 
		// 	g_data.t[i].circumcircle.y, 
		// 	g_data.t[i].circumcircle.r, 
		// 	100);
		// drawCircle(g_data.t[i], 100);
	}
	glEnd();
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	(void)x;
	(void)y;

	switch (key)
	{
		case 'q':
		case 'Q':
		case '\033':  // ESC
			glutDestroyWindow(g_data.gWindowID);
			std::exit(EXIT_SUCCESS);
			return;
		case 'x':
			g_data.radX += M_PI / 12;
			glutPostRedisplay();
			return ;
		case 'y':
			g_data.radY += M_PI / 12;
			glutPostRedisplay();
			return ;
		case 'z':
			g_data.radZ += M_PI / 12;
			glutPostRedisplay();
			return ;
		case 't':
			g_data.radX = 0;
			g_data.radY = 0;
			g_data.radZ = 0;
			glutPostRedisplay();
			return ;
		default:
			break;
	}
}

bool	TerrestrialSimulation::SimulationStart(int argc, 
											   char** argv, 
											   const std::string &fileName)
{
	int64_t	maxHeight;
	int64_t	minHeight;

	if (ParseModFile(fileName, this->_specificPoints, maxHeight, minHeight) == false)
	{
		return false;
	}
	this->DecideMapSize();

	DT	dT(this->_specificPoints, this->_mapSize);
	int64_t **map = new int64_t*[this->_mapSize[X]];

	for (size_t	i = 0; i < this->_mapSize[X]; ++i)
	{
		map[i] = new int64_t[this->_mapSize[Y]];
	}

	// dT.Calculation(map);

	g_data.map = map;
	g_data.t = dT.Calculation();
	g_data.mapSize[X] = this->_mapSize[X];
	g_data.mapSize[Y] = this->_mapSize[Y];
	g_data.maxHeight = maxHeight;
	g_data.minHeight = minHeight;
	g_data.radX = asin(sqrt(2) / sqrt(3));
	g_data.radY = 0;
	g_data.radZ = M_PI_4;
	g_data.scaling = SCALING;

	Graphic g = Graphic(argc, argv, SCREEN_SIZE_X, SCREEN_SIZE_Y);
	g_data.gWindowID = g.gWindowID;
	g.KeyboardFunc(keyboard);
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