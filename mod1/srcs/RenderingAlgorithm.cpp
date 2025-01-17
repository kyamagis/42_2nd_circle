#include <GL/glut.h>

void	RenderingAlgorithm()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3d(1.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);

	glVertex2d(-0.5, 0.5);
	glVertex2d( 0.5, 0.5);
	glVertex2d( 0.5,-0.5);

	glEnd();
	glFlush();
}