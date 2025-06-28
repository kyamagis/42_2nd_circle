#ifndef Graphic_HPP
# define Graphic_HPP

#include <GL/glut.h>
#include <GL/freeglut.h>

// #include <deque>
#include "./Triangle.hpp"
#include "./MPS.hpp"

class Graphic
{
	private:
		Graphic();

		void	_InitGraphicData(const std::deque<Triangle> &ts, 
								const uint32_t mapSize[3],
								const int64_t maxHeight,
								const int64_t minHeight);
		
	public:
		Graphic(const int argc, const char** argv, const int sizeX, const int sizeY);
		Graphic(const int argc, const char** argv, const int sizeX, const int sizeY, 
				const std::deque<Triangle> &ts, const uint32_t mapSize[3],
				const int64_t maxHeight, const int64_t minHeight);
		~Graphic();

		void	GraphicLoop(void (*func)(void));
		void	GraphicLoop(void);
		void	KeyboardFunc(void (*func)(unsigned char key, int x, int y));
		
		// void	RenderingAlgorithm();
};
void	drawVertex(const Vec &vertex);
void	onWindowClose(void);

#endif
