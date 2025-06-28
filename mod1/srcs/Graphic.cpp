#include <cmath>

#include "../includes/Graphic.hpp"
#include "../includes/Print.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Defines.hpp"
#include "../includes/Quaternion.hpp"

typedef struct s_data
{
	Vec						mapSize;
	Vec						halfMapSize;
	std::deque<Triangle>	ts;
	MPS						*mps;

	int64_t	maxHeight;
	int64_t	minHeight;
	double	midHeight;
	Vec		rad;
	Quaternion q;
	double	scaling;
	size_t	i;
	size_t	count;
	Vec		rotatedVertex;
	bool	circleFlg;
	bool	lineFlg;
	bool	visibleBucketsFlg;
	bool	simulationFlg;
	int		gWindowID;

	unsigned char	key;
	int				mouseX;
	int				mouseY;
	int				windowSizeX;
	int				windowSizeY;
	size_t			elapsedTime;

} t_data;

static t_data g_data;

// https://tokoik.github.io/opengl/libglut.html

void defaultkeyboard(unsigned char key, int x, int y)
{
	(void)x;
	(void)y;

	switch (key)
	{
		case 'q':
		case 'Q':
		case 3:
		case '\033':  // ESC
			glutDestroyWindow(g_data.gWindowID);
			std::exit(EXIT_SUCCESS);
			return;
		default:
			break;
	}
}

void	drawVertex(const Vec &vertex)
{
	const Vec		rotatedPos = vertex.Rotate(g_data.q);
	const double	coordinateX = rotatedPos.x / g_data.mapSize.x;
	const double	coordinateY =  - 1.0 * (rotatedPos.y / g_data.mapSize.y);
	const double	coordinateZ = rotatedPos.z / g_data.midHeight;
	const double	mousePosX = double(g_data.mouseX) / double(g_data.windowSizeX) - 0.5;
	const double	mousePosY = -(double(g_data.mouseY) / double(g_data.windowSizeY) - 0.5);

	glVertex3f((coordinateX - mousePosX) * g_data.scaling, 
			   (coordinateY - mousePosY) * g_data.scaling, 
			   -coordinateZ * DEPTH_SCALING * g_data.scaling);
}

void mouseWheel(int button, int dir, int x, int y)
{
	(void)button;
	(void)x;
	(void)y;

	Print::Err(std::to_string(g_data.scaling));
	if (dir > 0)
	{
		
	}
	else
	{

	}
	
	return;
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (g_data.scaling < 3.0)
		{
			g_data.scaling += 0.1;
		}
		g_data.mouseX = x;
		g_data.mouseY = y;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (0.1 < g_data.scaling)
		{
			g_data.scaling -= 0.1;
		}
	}
	else if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		if (0.1 < g_data.scaling)
		{
			g_data.scaling -= 0.1;
		}
	}

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	(void)x;
	(void)y;
	g_data.key = key;
	switch (key)
	{
		case 'b':
			g_data.visibleBucketsFlg = !g_data.visibleBucketsFlg;
			break;
		case 'c':
			g_data.q = 0.0;
			g_data.circleFlg = !g_data.circleFlg;
			break;
		case 'i':
			g_data.mouseX = g_data.windowSizeX / 2;
			g_data.mouseY = g_data.windowSizeY / 2;
			g_data.q = 0.0;
			g_data.q = g_data.q.calcQuaternion(M_PI / 12 * 3, Vec(0,0,1));
			g_data.q = g_data.q.calcQuaternion(M_PI / 12 * 5, Vec(1,0,0));
			g_data.scaling = SCALING;
			g_data.elapsedTime = 0;
			break;
		case 'l':
			g_data.lineFlg = !g_data.lineFlg;
			break;
		case 'n':
			if (g_data.i + 1 < g_data.ts.size())
			{
				++g_data.i;
			} 
			std::cout << "i: " << g_data.i << g_data.ts[g_data.i] << std::endl;
			break;
		case 'p':
			if (0 < g_data.i)
			{
				--g_data.i;
			}
			std::cout << "i: " << g_data.i << g_data.ts[g_data.i] << std::endl;
			break;
		case 'q':
		case 'Q':
		case 3:  // Ctrl + C
		case '\033':  // ESC
			glutDestroyWindow(g_data.gWindowID);
			std::exit(EXIT_SUCCESS);
			return;
		case 's':
			if(g_data.elapsedTime + 1 < SIMULATION_TIME)
			{
				++g_data.elapsedTime;
			}
			break;
		case 'S':
			if(0 < g_data.elapsedTime)
			{
				--g_data.elapsedTime;
			}
			break;
		case 19: // ctrl s
			g_data.elapsedTime = 0;
			g_data.simulationFlg = !g_data.simulationFlg;
			break;
		case 't':
			g_data.mouseX = g_data.windowSizeX / 2;
			g_data.mouseY = g_data.windowSizeY / 2;
			g_data.q = 0.0;
			break;
		case 'x':
			g_data.q = g_data.q.calcQuaternion(RADIAN, Vec(1,0,0));
			break;
		case 'X':
			g_data.q = g_data.q.calcQuaternion(-RADIAN, Vec(1,0,0));
			break;
		case 'y':
			g_data.q = g_data.q.calcQuaternion(RADIAN, Vec(0,1,0));
			break;
		case 'Y':
			g_data.q = g_data.q.calcQuaternion(-RADIAN, Vec(0,1,0));
			break;
		case 'z':
			g_data.q = g_data.q.calcQuaternion(RADIAN, Vec(0,0,1));
			break;
		case 'Z':
			g_data.q = g_data.q.calcQuaternion(-RADIAN, Vec(0,0,1));
			break;			
		default:
			return ;
	}
	glutPostRedisplay();
}

void	RenderingAlgorithm()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // カラー & Zバッファーをクリア

	if (g_data.visibleBucketsFlg)
	{
		g_data.mps->DrawDisFromWallSQ(g_data.halfMapSize, g_data.midHeight);
	}
	g_data.mps->DrawParticles(g_data.halfMapSize, g_data.midHeight, g_data.elapsedTime);
	for (size_t	i = 0; i < g_data.i; ++i)
	{
		if (g_data.circleFlg == true)
		{
			g_data.ts[i].circumcircle.DrawCircle2d(g_data.halfMapSize, 
													g_data.midHeight, 100);
		}
		g_data.ts[i].DrawTriangle(g_data.maxHeight,
									g_data.minHeight,
									g_data.midHeight,
									g_data.halfMapSize,
									g_data.lineFlg);
	
	}
	glFlush();
	
	if (g_data.simulationFlg)
	{
		if(g_data.elapsedTime + 1 < SIMULATION_TIME)
		{
			++g_data.elapsedTime;
		}
		else
		{
			g_data.elapsedTime = 0;
		}
		glutPostRedisplay();
	}
}

void	onWindowClose(void)
{
	glutDestroyWindow(g_data.gWindowID);
	std::exit(EXIT_SUCCESS);
}

Graphic::Graphic()
{
	
}

Graphic::Graphic(const int argc, const char** argv, const int sizeX, const int sizeY)
{
	glutInit(const_cast<int*>(&argc), const_cast<char**>(argv));

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(sizeX, sizeY);
	glutInitWindowPosition(300, 100);

	g_data.gWindowID = glutCreateWindow("mod1");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutWMCloseFunc(onWindowClose);
}

Graphic::Graphic(const int argc, const char** argv, const int sizeX, const int sizeY, 
				const std::deque<Triangle> &ts, const uint32_t mapSize[3],
				const int64_t maxHeight, const int64_t minHeight)
{
	glutInit(const_cast<int*>(&argc), const_cast<char**>(argv));

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(sizeX, sizeY);
	g_data.windowSizeX = sizeX;
	g_data.windowSizeY = sizeY;
	glutInitWindowPosition(300, 100);

	this->_InitGraphicData(ts, mapSize, maxHeight, minHeight);

	g_data.gWindowID = glutCreateWindow("mod1");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutWMCloseFunc(onWindowClose);
}

Graphic::~Graphic()
{
	Print::Out("des");
	delete g_data.mps;
}

void	Graphic::GraphicLoop(void (*func)(void))
{
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMouseWheelFunc(mouseWheel);
	glutDisplayFunc(func);
	glutMainLoop();
}

void	Graphic::GraphicLoop(void)
{
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMouseWheelFunc(mouseWheel);
	glutDisplayFunc(RenderingAlgorithm);
	glutMainLoop();
}

double	extend_map(const uint32_t mapSize)
{	
	const uint32_t	num = (uint32_t)((mapSize) / BUCKET_LENGTH);
	const double	diff = mapSize - num * BUCKET_LENGTH;

	return mapSize + BUCKET_LENGTH - diff + 4 * BUCKET_LENGTH;
}

void	Graphic::_InitGraphicData(const std::deque<Triangle> &ts, 
								 const uint32_t mapSize[3],
								 const int64_t maxHeight,
								 const int64_t minHeight)
{
	g_data.mapSize.x = extend_map(mapSize[X]);
	g_data.mapSize.y = extend_map(mapSize[Y]);
	g_data.mapSize.z = extend_map(mapSize[Z]);
	g_data.halfMapSize.x = g_data.mapSize.x / 2.0;
	g_data.halfMapSize.y = g_data.mapSize.y / 2.0;
	g_data.halfMapSize.z = g_data.mapSize.z / 2.0;
	g_data.ts = ts;
	for (size_t	i = 0; i < g_data.ts.size(); ++i)
	{
		g_data.ts[i].a.x += 2.0 * BUCKET_LENGTH;
		g_data.ts[i].a.y += 2.0 * BUCKET_LENGTH;
		g_data.ts[i].b.x += 2.0 * BUCKET_LENGTH;
		g_data.ts[i].b.y += 2.0 * BUCKET_LENGTH;
		g_data.ts[i].c.x += 2.0 * BUCKET_LENGTH;
		g_data.ts[i].c.y += 2.0 * BUCKET_LENGTH;
		g_data.ts[i].circumcircle.center.x += 2.0 * BUCKET_LENGTH;
		g_data.ts[i].circumcircle.center.y += 2.0 * BUCKET_LENGTH;
	}
	add_bottom(g_data.ts, Vec(mapSize[X],mapSize[Y],mapSize[Z]),
					g_data.mapSize,
					2.0 * BUCKET_LENGTH);
	add_cube(g_data.ts, g_data.mapSize);
	g_data.mps = new MPS(g_data.mapSize, g_data.ts);

	g_data.maxHeight = maxHeight;
	g_data.minHeight = minHeight;
	g_data.midHeight = (g_data.maxHeight + g_data.minHeight) / 2.0;
	g_data.rad.x = 0.0;
	g_data.rad.y = 0.0;
	g_data.rad.z = 0.0;
	g_data.q = g_data.q.calcQuaternion(M_PI / 12 * 3, Vec(0,0,1));
	g_data.q = g_data.q.calcQuaternion(M_PI / 12 * 5, Vec(1,0,0));
	g_data.i = g_data.ts.size();
	g_data.count = 0;
	g_data.scaling = SCALING;
	g_data.circleFlg = false;
	g_data.lineFlg = false;
	g_data.visibleBucketsFlg = false;
	g_data.simulationFlg = false;
	g_data.key = 'i';
	g_data.mouseX = g_data.windowSizeX / 2;
	g_data.mouseY = g_data.windowSizeY / 2;
	g_data.elapsedTime = 0;
}

void	Graphic::KeyboardFunc(void (*func)(unsigned char key, int x, int y))
{
	glutKeyboardFunc(func);
}
