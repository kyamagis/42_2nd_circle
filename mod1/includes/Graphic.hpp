#ifndef Graphic_HPP
# define Graphic_HPP

class Graphic
{
	private:
		Graphic();

	public:
		Graphic(int argc, char** argv);
		~Graphic();

		void	GraphicLoop(void (*func)(void));
		// void	RenderingAlgorithm();
};

void	keyboard(unsigned char key, int x, int y);
void	onWindowClose(void);

#endif