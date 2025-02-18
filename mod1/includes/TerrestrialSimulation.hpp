#ifndef TerrestrialSimulation_HPP
# define TerrestrialSimulation_HPP


#include <deque>

#include "../includes/Triangle.hpp"
#include "./Vec.hpp"

#define X 0
#define Y 1
#define Z 0
#define IS_SPECIFIED 1
#define TRUE 1
#define FALSE 0

class TerrestrialSimulation
{
	private:
		std::deque<Vec> _specificPoints;
		uint32_t		_mapSize[2];
		int32_t			***_threeDCoordinates;

		
	public:
		TerrestrialSimulation();
		~TerrestrialSimulation();
	
		bool	SimulationStart(int argc, 
								char** argv, 
								const std::string &fileName);
		void	DecideMapSize();
		void	PrintSpecificPoints();

};


#endif