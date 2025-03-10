#ifndef TerrestrialSimulation_HPP
# define TerrestrialSimulation_HPP

#include <deque>

#include "../includes/Triangle.hpp"
#include "./Vec.hpp"
#include "./Defines.hpp"

class TerrestrialSimulation
{
	private:
		std::deque<Vec> _specificPoints;
		uint32_t		_mapSize[3];
		int64_t			_maxHeight;
		int64_t			_minHeight;

		bool	_ReadMapData(const std::string &fileName);
		void	_DrawSimulation(const int argc, 
								const char** argv, 
								const std::deque<Triangle> &ts);

	public:
		TerrestrialSimulation();
		~TerrestrialSimulation();
	
		bool	SimulationStart(const int argc, 
								const char** argv, 
								const std::string &fileName);
		void	PrintSpecificPoints(void);

};


#endif