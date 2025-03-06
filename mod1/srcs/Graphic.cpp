#include "../includes/Graphic.hpp"
#include "../includes/Print.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Defines.hpp"

#include <cmath>

typedef struct s_data
{
	int64_t 				**map;
	uint32_t				mapSize[3];
	Vec						halfMapSize;
	std::deque<Triangle>	ts;
	std::deque<Triangle>	origenTs;
	MPS						*mps;

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
		case '\033':  // ESC
			glutDestroyWindow(g_data.gWindowID);
			std::exit(EXIT_SUCCESS);
			return;
		default:
			break;
	}
}

void	rotation(Vec &vertex)
{
	switch (g_data.key)
	{
		case 'i':
			// if (g_data.count == 0)
			// {
			// 	++g_data.count;
			// 	glutPostRedisplay();
			// 	break;
			// }
			vertex.RotationZ(g_data.radZ);
			vertex.RotationX(g_data.radX);
			break;
		case 'x':
			vertex.RotationX(M_PI / 12.0);
			break;
		case 'X':
			vertex.RotationX(-M_PI / 12.0);
			break;
		case 'y':
			vertex.RotationY(M_PI / 12);
			break;
		case 'Y':
			vertex.RotationY(-M_PI / 12);
			break;
		case 'z':
			vertex.RotationZ(M_PI / 12);
			break;
		case 'Z':
			vertex.RotationZ(-M_PI / 12);
			break;
		case 'c':
		default:
			break;
	}
}

void	drawVertex(const Vec &vertex)
{
	double	coordinateX = vertex.x / double(g_data.mapSize[X]);
	double	coordinateY =  - 1.0 * (vertex.y / double(g_data.mapSize[Y]));
	double	coordinateZ = vertex.z / double(g_data.midHeight);

	glVertex3f(coordinateX * g_data.scaling, 
			   coordinateY * g_data.scaling, 
			   -coordinateZ * DEPTH_SCALING * g_data.scaling);
}

void	RenderingAlgorithm()
{
	// std::cout << "RenderingAlgorithm " << g_data.radY++ << std::endl;
	if (g_data.key == 'i')
	{
		g_data.ts = g_data.origenTs;
	}
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // カラー & Zバッファーをクリア
	
	if (g_data.ts.size() < g_data.i)
	{
		g_data.i = g_data.ts.size();
	}
	g_data.mps->RotationBs();
	g_data.mps->RotationPs();
	rotation_ts(g_data.ts);
	if (g_data.visibleBucketsFlg)
	{
		g_data.mps->DrawDisFromWall();
	}
	g_data.mps->DrawParticles();
	for (size_t	i = 0; i < g_data.i; ++i)
	{
		if (g_data.circleFlg == true)
		{
			g_data.ts[i].circumcircle.DrawCircle2d(g_data.mapSize, 
													g_data.scaling, 100);
		}
		g_data.ts[i].DrawTriangle(g_data.maxHeight,
									g_data.minHeight,
									g_data.midHeight,
									g_data.lineFlg,
									g_data.origenTs[i].a.z,
									g_data.origenTs[i].b.z,
									g_data.origenTs[i].c.z);
	
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

void keyboard(unsigned char key, int x, int y)
{
	(void)x;
	(void)y;

	g_data.key = key;

	switch (key)
	{
		case 'b':
			g_data.visibleBucketsFlg = !g_data.visibleBucketsFlg;
			glutPostRedisplay();
			return ;
		case 'c':
			g_data.ts = g_data.origenTs;
			g_data.circleFlg = !g_data.circleFlg;
			glutPostRedisplay();
			return ;
		case 'i':
			g_data.scaling = SCALING;
			glutPostRedisplay();
			return ;
		case 'l':
			g_data.lineFlg = !g_data.lineFlg;
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
		case 'q':
		case 'Q':
		case '\033':  // ESC
			glutDestroyWindow(g_data.gWindowID);
			std::exit(EXIT_SUCCESS);
			return;
		case 's':
			g_data.mps->NavierStokesEquations();
			glutPostRedisplay();
			return ;
		case 't':
			g_data.ts = g_data.origenTs;
			glutPostRedisplay();
			return ;
		case 'x':
		case 'X':
		case 'y':
		case 'Y':
		case 'z':
		case 'Z':
			glutPostRedisplay();
			return ;
		default:
			break;
	}
}

void	MoveOToMapCenter(std::deque<Triangle> &ts)
{
	for (size_t	i = 0; i < ts.size(); ++i)
	{
		ts[i].MoveVertexToMapCenter(g_data.halfMapSize, g_data.midHeight);
	}
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
	MoveOToMapCenter(g_data.ts);
	g_data.origenTs = g_data.ts;
	g_data.mps->MoveVertexToMapCenterBs(g_data.halfMapSize, g_data.midHeight);
	g_data.mps->MoveVertexToMapCenterPs(g_data.halfMapSize, g_data.midHeight);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMouseWheelFunc(mouseWheel);
	glutDisplayFunc(func);
	glutMainLoop();
}

void	Graphic::GraphicLoop(void)
{
	MoveOToMapCenter(g_data.ts);
	g_data.origenTs = g_data.ts;
	g_data.mps->MoveVertexToMapCenterBs(g_data.halfMapSize, g_data.midHeight);
	g_data.mps->MoveVertexToMapCenterPs(g_data.halfMapSize, g_data.midHeight);
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
	g_data.radX = M_PI / 12.0 * 5.0;
	g_data.radY = 0;
	g_data.radZ = M_PI_4;
	g_data.ts.push_back({Vec(0,0,-EPS), 
						 Vec(0,g_data.mapSize[Y] - 1,-EPS), 
						 Vec(g_data.mapSize[X] - 1,0,-EPS), 
						 false});
	g_data.ts.push_back({Vec(g_data.mapSize[X] - 1,g_data.mapSize[Y] - 1,-EPS), 
						 Vec(0,g_data.mapSize[Y] - 1,-EPS), 
						 Vec(g_data.mapSize[X] - 1,0,-EPS), 
						 false});
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