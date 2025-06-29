#ifndef Graphic_HPP
# define Graphic_HPP

#include <GL/glut.h>
#include <GL/freeglut.h>

#include "./Triangle.hpp"
#include "./MPS.hpp"

class Graphic
{
	private:
		Graphic();

		void	_InitGraphicData(const t_data	&data);
		
	public:
		Graphic(const int argc, const char** argv, const int sizeX, const int sizeY);
		Graphic(const int argc, const char** argv, const int sizeX, const int sizeY,
				const t_data	&data);
		~Graphic();

		void	GraphicLoop(void (*func)(void));
		void	GraphicLoop(void);
		void	KeyboardFunc(void (*func)(unsigned char key, int x, int y));
};

void	onWindowClose(void);

#endif
