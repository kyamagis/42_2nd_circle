#include "../includes/TerrestrialSimulation.hpp"
#include "../includes/ParseModFile.hpp"
#include "../includes/Graphic.hpp"
#include "../includes/Print.hpp"
#include "../includes/DelaunayTriangulation.hpp"
#include "../includes/MathUtils.hpp"

#define X 0
#define Y 1

#define SCREEN_SIZE_X 1000
#define SCREEN_SIZE_Y 1000

#define SCALING 0.5

#include <string.h>
#include <GL/glut.h>
#include <cmath>

typedef struct s_data
{
	int64_t 				**map;
	uint32_t				mapSize[2];
	double					halfMapsize[2];
	std::deque<Triangle>	ts;
	std::deque<Triangle>	origenTs;
	int64_t	maxHeight;
	int64_t	minHeight;
	double	midHeight;
	double	shrinkageRatioX;
	double	shrinkageRatioY;
	double	radX;
	double	radY;
	double	radZ;
	double	scaling;
	size_t	i;
	Vec		rotatedVertex;
	bool	circleFlg;
	int		gWindowID;
	unsigned char key;
} t_data;

static t_data g_data;

TerrestrialSimulation::TerrestrialSimulation(): _threeDCoordinates(NULL)
{

}

TerrestrialSimulation::~TerrestrialSimulation()
{

}

void	Vertex(Vec &vertex)
{
	switch (g_data.key)
	{
		case 'i':
			z_rotation(vertex, g_data.radZ , vertex.x, vertex.y);
			x_rotation(vertex, g_data.radX , vertex.y, vertex.z);
			break;
		case 'x':
			x_rotation(vertex, M_PI / 12.0, vertex.y, vertex.z);
			break;
		case 'X':
			x_rotation(vertex, -M_PI / 12.0, vertex.y, vertex.z);
			break;
		case 'y':
			y_rotation(vertex, M_PI / 12, vertex.x, vertex.z);
			break;
		case 'Y':
			y_rotation(vertex, -M_PI / 12, vertex.x, vertex.z);
			break;
		case 'z':
			z_rotation(vertex, M_PI / 12, vertex.x, vertex.y);
			break;
		case 'Z':
			z_rotation(vertex, -M_PI / 12, vertex.x, vertex.y);
			break;
		case 'c':
		default:
			break;
	}

	double	coordinateX = vertex.x / double(g_data.mapSize[X]);
	double	coordinateY =  - 1.0 * (vertex.y / double(g_data.mapSize[Y]));
	double	coordinateZ = vertex.z / double(g_data.maxHeight);

	glVertex3f(coordinateX * g_data.scaling, 
			   coordinateY * g_data.scaling, 
			   -coordinateZ * g_data.scaling);
}

void	triangleGradation(const int64_t height)
{
	// double	ratio, ratio2;
	double	ratio;

	if (height <= g_data.midHeight)
	{
		ratio = height - g_data.minHeight / (g_data.midHeight - g_data.minHeight);
		glColor3f(0.0, ratio, 1 - ratio);
	}
	else
	{
		ratio = height - g_data.midHeight / (g_data.maxHeight - g_data.midHeight);
		glColor3f(ratio, 1 - ratio, 0.0);
	}
}

void	drawTriangle(Triangle &t, 
					const int64_t heightA, 
					const int64_t heightB, 
					const int64_t heightC)
{
	// glBegin(GL_LINE_LOOP);
	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLES);

	triangleGradation(heightA);
	Vertex(t.a);
	triangleGradation(heightB);
	Vertex(t.b);
	triangleGradation(heightC);
	Vertex(t.c);
	glEnd();
}

void drawCircle(double cx, double cy, double r, int num_segments) {
	glBegin(GL_LINE_LOOP); // 円を線で描く
	glColor3f(0.0, 1.0, 0.0);

	cx = double(cx) / g_data.mapSize[X];
	cy = - 1.0 * (double(cy) / g_data.mapSize[Y]);
	r = sqrt(r) / g_data.mapSize[X];

	for (int i = 0; i < num_segments; i++) {
		double theta = 2.0 * M_PI * double(i) / double(num_segments); // 角度
		double x = r * cos(theta); // X座標
		double y = r * sin(theta); // Y座標
		glVertex2d((cx + x) * g_data.scaling, (cy + y) * g_data.scaling);
	}
	glEnd();
}

void drawCircle(const Triangle &t, int num_segments) 
{
	drawCircle(t.circumcircle.x, t.circumcircle.y, t.circumcircle.r, num_segments);
}

void	RenderingAlgorithm()
{
	std::cout << "RenderingAlgorithm " << g_data.radY++ << std::endl;
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // カラー & Zバッファーをクリア
		// glColor3f(1.0, 0.0, 0.0);
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
	if (g_data.ts.size() < g_data.i)
	{
		g_data.i = g_data.ts.size();
	}
	for (size_t	i = 0; i < g_data.i; ++i)
	{
		drawTriangle(g_data.ts[i], 
					g_data.origenTs[i].a.z,
					g_data.origenTs[i].b.z,
					g_data.origenTs[i].c.z);
		if (g_data.circleFlg == true)
		{
			drawCircle(g_data.ts[i], 100);
		}
	}
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	(void)x;
	(void)y;

	g_data.circleFlg = false;
	g_data.key = key;

	switch (key)
	{
		case 'q':
		case 'Q':
		case '\033':  // ESC
			glutDestroyWindow(g_data.gWindowID);
			std::exit(EXIT_SUCCESS);
			return;
		case 'x':
		case 'X':
		case 'y':
		case 'Y':
		case 'z':
		case 'Z':
			glutPostRedisplay();
			return ;
		case 'i':
		case 't':
			g_data.ts = g_data.origenTs;
			glutPostRedisplay();
			return ;
		case 'c':
			g_data.ts = g_data.origenTs;
			g_data.circleFlg = true;
			glutPostRedisplay();
			return ;
		case 'n':
			++g_data.i;
			std::cout << "i: " << g_data.i << g_data.ts[g_data.i] << std::endl;
			glutPostRedisplay();
			return ;
		case 'p':
			if (0 < g_data.i)
			{
				--g_data.i;
			}
			std::cout << "i: " << g_data.i << g_data.ts[g_data.i] << std::endl;
			glutPostRedisplay();
			return ;
		default:
			break;
	}
}

void	OMoveToMapCenter(std::deque<Triangle> &ts)
{
	for (size_t	i = 0; i < ts.size(); ++i)
	{
		ts[i].a.x -= g_data.halfMapsize[X];
		ts[i].a.y -= g_data.halfMapsize[Y];
		ts[i].a.z -= g_data.midHeight / 2.0;
		ts[i].b.x -= g_data.halfMapsize[X];
		ts[i].b.y -= g_data.halfMapsize[Y];
		ts[i].b.z -= g_data.midHeight / 2.0;
		ts[i].c.x -= g_data.halfMapsize[X];
		ts[i].c.y -= g_data.halfMapsize[Y];
		ts[i].c.z -= g_data.midHeight / 2.0;

		ts[i].circumcircle.x -= g_data.halfMapsize[X];
		ts[i].circumcircle.y -= g_data.halfMapsize[Y];

	}
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
	DT	dT(this->_specificPoints, this->_mapSize);
	// int64_t **map = new int64_t*[this->_mapSize[X]];

	// for (size_t	i = 0; i < this->_mapSize[X]; ++i)
	// {
	// 	map[i] = new int64_t[this->_mapSize[Y]];
	// }

	// dT.Calculation(map);

	// g_data.map = map;
	
	int64_t	maxHeight = 0;
	int64_t	minHeight = 0;

	g_data.ts = dT.Calculation(maxHeight, minHeight);
	
	g_data.mapSize[X] = this->_mapSize[X];
	g_data.mapSize[Y] = this->_mapSize[Y];
	g_data.halfMapsize[X] = this->_mapSize[X] / 2.0;
	g_data.halfMapsize[Y] = this->_mapSize[Y] / 2.0;
	g_data.maxHeight = maxHeight;
	g_data.minHeight = minHeight;
	g_data.midHeight = (maxHeight + minHeight) / 2.0;
	g_data.shrinkageRatioX = 1.0 / g_data.mapSize[X] * 2.0;
	g_data.shrinkageRatioY = 1.0 / g_data.mapSize[Y] * 2.0;
	g_data.radX = M_PI / 12.0 * 5.0;
	g_data.radY = 0;
	g_data.radZ = M_PI_4;
	g_data.i = g_data.ts.size();
	g_data.scaling = SCALING;
	g_data.circleFlg = false;
	g_data.key = 'i';

	Graphic g = Graphic(argc, argv, SCREEN_SIZE_X, SCREEN_SIZE_Y);
	g_data.gWindowID = g.gWindowID;
	OMoveToMapCenter(g_data.ts);
	g_data.origenTs = g_data.ts;
	g.KeyboardFunc(keyboard);;
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