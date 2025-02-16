#include "../includes/Graphic.hpp"
#include "../includes/Print.hpp"

static int	gWindowID;

// https://tokoik.github.io/opengl/libglut.html

Graphic::Graphic()
{
	
}

void keyboard(unsigned char key, int x, int y);

Graphic::Graphic(int argc, char** argv, int	sizeX, int	sizeY)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(sizeX, sizeY);
	glutInitWindowPosition(300, 50);

	gWindowID = glutCreateWindow("mod1");
	glutInitDisplayMode(GLUT_RGBA);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutKeyboardFunc(keyboard);
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

void keyboard(unsigned char key, int x, int y)
{
	(void)x;
	(void)y;

	switch (key)
	{
		case 'q':
		case 'Q':
		case '\033':  // ESC
			glutDestroyWindow(gWindowID);
			std::exit(EXIT_SUCCESS);
			return;
		default:
			break;
	}
}

void	onWindowClose(void)
{
	glutDestroyWindow(gWindowID);
	std::exit(EXIT_SUCCESS);
}