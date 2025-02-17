#ifndef Graphic_HPP
# define Graphic_HPP

#include <GL/glut.h>
#include <GL/freeglut.h>

class Graphic
{
	private:
		Graphic();
		
		public:
		Graphic(int argc, char** argv, int	sizeX, int	sizeY);
		~Graphic();
		
		int	gWindowID;
		void	GraphicLoop(void (*func)(void));
		void	KeyboardFunc(void (*func)(unsigned char key, int x, int y));
		// void	RenderingAlgorithm();
};

void	onWindowClose(void);

#endif