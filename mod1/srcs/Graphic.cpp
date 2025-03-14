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
	int		gWindowID;

	unsigned char key;

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
	const Vec	rotatedPos = vertex.Rotate(g_data.q);

	const double	coordinateX = rotatedPos.x / g_data.mapSize.x;
	const double	coordinateY =  - 1.0 * (rotatedPos.y / g_data.mapSize.y);
	const double	coordinateZ = rotatedPos.z / g_data.midHeight;

	glVertex3f(coordinateX * g_data.scaling, 
			   coordinateY * g_data.scaling, 
			   -coordinateZ * DEPTH_SCALING * g_data.scaling);
}

void	RenderingAlgorithm()
{
	// std::cout << "RenderingAlgorithm " << g_data.radY++ << std::endl;
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // カラー & Zバッファーをクリア

	if (g_data.visibleBucketsFlg)
	{
		g_data.mps->DrawDisFromWallSQ(g_data.halfMapSize, g_data.midHeight);
	}
	g_data.mps->DrawParticles(g_data.halfMapSize, g_data.midHeight);
	
	for (size_t	i = 0; i < g_data.i; ++i)
	{
		if (g_data.circleFlg == true)
		{
			g_data.ts[i].circumcircle.DrawCircle2d(g_data.halfMapSize, 
													g_data.midHeight, 100);
		}
		// g_data.ts[i].DrawNormalVector(g_data.midHeight, g_data.halfMapSize);
		g_data.ts[i].DrawTriangle(g_data.maxHeight,
									g_data.minHeight,
									g_data.midHeight,
									g_data.halfMapSize,
									g_data.lineFlg);
	
	}
	g_data.key = 0;
	glFlush();
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
	(void)x;
	(void)y;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (g_data.scaling < 3.0)
		{
			g_data.scaling += 0.1;
		}
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
	// printf(" at (%d, %d)\n", x, y);
}

Quaternion	makeQuaternion(const double radian, const Vec &direction)
{
	return Quaternion(cos(radian / 2.0), direction * sin(radian / 2.0));
}

Quaternion calcQuaternion(const Quaternion &qGlobal, const double radian, const Vec &direction)
{
	Quaternion q = makeQuaternion(radian, direction);
	if (qGlobal == 0.0)
	{
		return q;
	}
	return q * qGlobal;
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
			g_data.q = 0.0;
			g_data.q = calcQuaternion(g_data.q, M_PI / 12 * 3, Vec(0,0,1));
			g_data.q = calcQuaternion(g_data.q, M_PI / 12 * 5, Vec(1,0,0));
			// g_data.q = calcQuaternion(g_data.q, M_PI_2, Vec(1,0,0));
			// g_data.q = calcQuaternion(g_data.q, M_PI_2, Vec(0,1,0));
			g_data.scaling = SCALING;
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
			g_data.mps->NavierStokesEquations();
			break;
		case 't':
			g_data.q = 0.0;
			break;
		case 'x':
			g_data.q = calcQuaternion(g_data.q, RADIAN, Vec(1,0,0));
			break;
		case 'X':
			g_data.q = calcQuaternion(g_data.q, -RADIAN, Vec(1,0,0));
			break;
		case 'y':
			g_data.q = calcQuaternion(g_data.q, RADIAN, Vec(0,1,0));
			break;
		case 'Y':
			g_data.q = calcQuaternion(g_data.q, -RADIAN, Vec(0,1,0));
			break;
		case 'z':
			g_data.q = calcQuaternion(g_data.q, RADIAN, Vec(0,0,1));
			break;
		case 'Z':
			g_data.q = calcQuaternion(g_data.q, -RADIAN, Vec(0,0,1));
			break;			
		default:
			return ;
	}
	glutPostRedisplay();
}

void	onWindowClose(void)
{
	glutDestroyWindow(g_data.gWindowID);
	std::exit(EXIT_SUCCESS);
}

Graphic::Graphic()
{
	
}

Graphic::Graphic(const int argc, const char** argv, int	sizeX, int	sizeY)
{
	glutInit(const_cast<int*>(&argc), const_cast<char**>(argv));

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	// glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(sizeX, sizeY);
	glutInitWindowPosition(300, 50);

	g_data.gWindowID = glutCreateWindow("mod1");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	// glutKeyboardFunc(defaultkeyboard);
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
	const uint32_t	num = (uint32_t)((mapSize - 1) / BUCKET_LENGTH);
	const double	diff = mapSize - 1 - num * BUCKET_LENGTH;

	return mapSize - 1 + BUCKET_LENGTH - diff;
}

void	Graphic::InitGraphicData(const std::deque<Triangle> &ts, 
								 const uint32_t mapSize[3],
								 const int64_t maxHeight,
								 const int64_t minHeight)
{
	g_data.mapSize.x = extend_map(mapSize[X]);
	g_data.mapSize.y = extend_map(mapSize[Y]);
	g_data.mapSize.z = extend_map(mapSize[Z]);

	// g_data.mapSize.x = mapSize[X];
	// g_data.mapSize.y = mapSize[Y];
	// g_data.mapSize.z = mapSize[Z];
	g_data.halfMapSize.x = g_data.mapSize.x / 2.0;
	g_data.halfMapSize.y = g_data.mapSize.y / 2.0;
	g_data.halfMapSize.z = g_data.mapSize.z / 2.0;
	g_data.ts = ts;
	add_cube(g_data.ts, g_data.mapSize);
	g_data.mps = new MPS(g_data.mapSize, g_data.ts);

	g_data.maxHeight = maxHeight;
	g_data.minHeight = minHeight;
	g_data.midHeight = (g_data.maxHeight + g_data.minHeight) / 2.0;
	g_data.rad.x = 0.0;
	g_data.rad.y = 0.0;
	g_data.rad.z = 0.0;
	// g_data.q = calcQuaternion(g_data.q, M_PI_2, Vec(1,0,0));
	// g_data.q = calcQuaternion(g_data.q, M_PI_2, Vec(0,1,0));
	g_data.q = calcQuaternion(g_data.q, M_PI / 12 * 3, Vec(0,0,1));
	g_data.q = calcQuaternion(g_data.q, M_PI / 12 * 5, Vec(1,0,0));
	g_data.i = g_data.ts.size();
	g_data.count = 0;
	g_data.scaling = SCALING;
	g_data.circleFlg = false;
	g_data.lineFlg = false;
	g_data.visibleBucketsFlg = false;
	g_data.key = 'i';
}

void	Graphic::KeyboardFunc(void (*func)(unsigned char key, int x, int y))
{
	glutKeyboardFunc(func);
}
