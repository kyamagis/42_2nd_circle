#ifndef TerrestrialSimulation_HPP
# define TerrestrialSimulation_HPP

#include <deque>

#include "./Vec.hpp"

class TerrestrialSimulation
{
	private:
		std::deque<Vec> _vecDeque;
		uint32_t		_mapSize[2];

		
	public:
		TerrestrialSimulation();
		~TerrestrialSimulation();
	
		bool	SimulationStart(int argc, 
								char** argv, 
								const std::string &fileName);
		void	DecideMapSize();						
		void	PrintVecDeque();
};


#endif