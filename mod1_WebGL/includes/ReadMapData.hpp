#ifndef READMAPDATA_HPP
# define READMAPDATA_HPP

#include <deque>
#include <string>
#include "./Vec.hpp"

class RMD
{
	private:
		const std::string	&_fileName;
		std::deque<Vec> 	_specificPoints;
		uint32_t			_mapSize[3];
		int64_t				_maxHeight;
		int64_t				_minHeight;

		RMD();

		void	_InsertionAndSort(int32_t x, int32_t y, int32_t z);
		bool	_StorePosition(const std::string &getLine, 
								const size_t line, 
								size_t &i);
		bool	_StoreVec(const std::string &getLine, const size_t line);
		bool	_ParseLines(std::ifstream &ifs);
		bool	_ParseModFile(void);
		
		void	_DecideMapSize(void);
		void	_AddEndPoints(void);
		void	_Add8Corners(const double calibrationHeight);

	public:

		RMD(const std::string &fileName);
		~RMD();

		bool	ReadStart(void);
		std::deque<Vec>	GetSpecificPoints(void);
		void	GetMapSize(uint32_t mapSize[3]);
		void	GetMaxMinHeight(int64_t &maxHeight, 
								int64_t &minHeight);
};

#endif