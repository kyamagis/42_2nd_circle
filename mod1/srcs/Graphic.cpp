#include <cmath>

#include "../includes/Graphic.hpp"
#include "../includes/Print.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Defines.hpp"
#include "../includes/Quaternion.hpp"


typedef struct s_data
{
	int64_t 				**map;
	uint32_t				mapSize[3];
	Vec						halfMapSize;
	std::deque<Triangle>	ts;
	MPS						*mps;

	int64_t	maxHeight;
	int64_t	minHeight;
	double	midHeight;
	double	shrinkageRatioX;
	double	shrinkageRatioY;
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
	Vec	rotatedPos = vertex.Rotate(g_data.q);

	const double	coordinateX = rotatedPos.x / double(g_data.mapSize[X]);
	const double	coordinateY =  - 1.0 * (rotatedPos.y / double(g_data.mapSize[Y]));
	const double	coordinateZ = rotatedPos.z / double(g_data.midHeight);

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
		g_data.mps->DrawDisFromWall(g_data.halfMapSize, g_data.midHeight);
	}
	g_data.mps->DrawParticles(g_data.halfMapSize, g_data.midHeight);
	
	for (size_t	i = 0; i < g_data.i; ++i)
	{
		if (g_data.circleFlg == true)
		{
			g_data.ts[i].circumcircle.DrawCircle2d(g_data.mapSize,
													g_data.halfMapSize, 
													g_data.scaling, 100);
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
			// g_data.rad.x = M_PI / 12.0 * 5.0;
			// g_data.rad.y = 0.0;
			// g_data.rad.z = M_PI_4;
			g_data.q = 0.0;
			g_data.q = calcQuaternion(g_data.q, RADIAN * 5.0, Vec(1,0,0));
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

void	Graphic::InitGraphicData(const std::deque<Triangle> &ts, 
								 const uint32_t mapSize[3],
								 const int64_t maxHeight,
								 const int64_t minHeight)
{
	g_data.ts = ts;
	g_data.mps = new MPS(mapSize, ts);
	g_data.mapSize[X] = mapSize[X];
	g_data.mapSize[Y] = mapSize[Y];
	g_data.mapSize[Z] = mapSize[Z];
	g_data.halfMapSize.x = mapSize[X] / 2.0;
	g_data.halfMapSize.y = mapSize[Y] / 2.0;
	g_data.halfMapSize.z = mapSize[Z] / 2.0;
	g_data.maxHeight = maxHeight;
	g_data.minHeight = minHeight;
	g_data.midHeight = (g_data.maxHeight + g_data.minHeight) / 2.0;
	g_data.shrinkageRatioX = 1.0 / g_data.mapSize[X] * 2.0;
	g_data.shrinkageRatioY = 1.0 / g_data.mapSize[Y] * 2.0;
	g_data.rad.x = 5.0 * M_PI / 12;
	g_data.rad.y = 0.0;
	g_data.rad.z = 0.0;
	g_data.q = calcQuaternion(g_data.q, RADIAN * 5.0, Vec(1,0,0));
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


void	onWindowClose(void)
{
	glutDestroyWindow(g_data.gWindowID);
	std::exit(EXIT_SUCCESS);
}


// glShadeModel(GL_SMOOTH);
// glBegin(GL_QUADS);
// glColor3f(1.0f, 0.0f, 0.0f);
// glVertex3f(-0.5, 0.5, 0);


// glColor3f(0.0f, 1.0f, 0.0f);
// glVertex3f(0.0, 0.0, 0);

// glColor3f(0.0f, 0.0f, 1.0f);
// glVertex3f(0.5, -0.5, 0);
// glVertex3f(-0.5, -0.5, 0);
// // glShadeModel(GL_SMOOTH);
// // glBegin(GL_QUADS);




// glColor3f(0.0f, 1.0f, 0.0f);
// glVertex3f(0.0, 0.0, 0);


// glEnd();


// void drawCircle(double cx, double cy, double r, int num_segments) {
// 	glBegin(GL_LINE_LOOP); // 円を線で描く
// 	glColor3f(0.0, 1.0, 0.0);

// 	cx = double(cx) / g_data.mapSize[X];
// 	cy = - 1.0 * (double(cy) / g_data.mapSize[Y]);
// 	r = sqrt(r) / g_data.mapSize[X];

// 	for (int i = 0; i < num_segments; i++) {
// 		double theta = 2.0 * M_PI * double(i) / double(num_segments); // 角度
// 		double x = r * cos(theta); // X座標
// 		double y = r * sin(theta); // Y座標
// 		glVertex2d((cx + x) * g_data.scaling, (cy + y) * g_data.scaling);
// 	}
// 	glEnd();
// }

// void drawCircle(const Triangle &t, int num_segments) 
// {
// 	drawCircle(t.circumcircle.center.x, t.circumcircle.center.y, t.circumcircle.r, num_segments);
// }