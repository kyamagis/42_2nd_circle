#ifndef TerrestrialSimulation_HPP
# define TerrestrialSimulation_HPP

#include <deque>

#include "./Triangle.hpp"
#include "./Defines.hpp"
#include "./Structs.hpp"

class TS
{
	private:
		std::deque<Vec> _specificPoints;
		uint32_t		_mapSize[3];
		int64_t			_maxHeight;
		int64_t			_minHeight;

		bool	_ReadMapData(const std::string &fileName);
		void	_DrawSimulation(const int argc, const char** argv, const t_data &data);

	public:
		TS();
		~TS();
	
		bool	SimulationStart(const int argc, 
								const char** argv, 
								const std::string &fileName);
		void	PrintSpecificPoints(void);

};


#endif