#ifndef Graphic_HPP
# define Graphic_HPP

#include <GL/glut.h>
#include <GL/freeglut.h>
#include <deque>

#include "../includes/Triangle.hpp"

class Graphic
{
	private:
		Graphic();
		
		public:
		Graphic(int argc, char** argv, int	sizeX, int	sizeY);
		~Graphic();

		void	GraphicLoop(void (*func)(void));
		void	GraphicLoop(void);
		void	KeyboardFunc(void (*func)(unsigned char key, int x, int y));
		void	InitGraphicData(const std::deque<Triangle> &ts, 
								const uint32_t mapSize[2],
								const int64_t maxHeight,
								const int64_t minHeight);
		// void	RenderingAlgorithm();
};

void	onWindowClose(void);

#endif