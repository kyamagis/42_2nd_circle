#include "../includes/Graphic.hpp"
#include "../includes/Print.hpp"

static int	g_gWindowID;

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
			glutDestroyWindow(g_gWindowID);
			std::exit(EXIT_SUCCESS);
			return;
		default:
			break;
	}
}

Graphic::Graphic()
{
	
}

Graphic::Graphic(int argc, char** argv, int	sizeX, int	sizeY)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	// glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(sizeX, sizeY);
	glutInitWindowPosition(300, 50);

	g_gWindowID = glutCreateWindow("mod1");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutKeyboardFunc(defaultkeyboard);
	glutWMCloseFunc(onWindowClose);
}

Graphic::~Graphic()
{
	Print::Out("des");
}

void	Graphic::GraphicLoop(void (*func)(void))
{
	glutDisplayFunc(func);
	glutMainLoop();
}

void	Graphic::KeyboardFunc(void (*func)(unsigned char key, int x, int y))
{
	glutKeyboardFunc(func);
}


void	onWindowClose(void)
{
	glutDestroyWindow(g_gWindowID);
	std::exit(EXIT_SUCCESS);
}