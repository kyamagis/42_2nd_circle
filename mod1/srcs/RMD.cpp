#include "../includes/Print.hpp"
#include "../includes/Vec.hpp"
#include "../includes/ReadMapData.hpp"
#include "../includes/Utils.hpp"

#include <fstream>
#include <string>
#include <ctype.h>

static std::string	CreateFileErrorMessage(const std::string &errorStr,
	const size_t line, 
	size_t i)
{
	return  "Error: mod1 file: " + errorStr
	+ ": line " + std::to_string(line) 
	+ ": column " + std::to_string(++i);
}

static bool	StringToNum(const std::string &getLine, 
const size_t line, 
size_t &i, 
int32_t &pos)
{
	size_t	startIndex = i;

	// if (getLine[i] == '-')
	// {

	// }
	for(; isdigit(getLine[i]); ++i)
	{
	pos *= 10;
	pos += getLine[i] - '0';
	if (pos < MIN_COORDINATE || MAX_COORDINATE < pos)
	{
	return Print::Err(CreateFileErrorMessage("Over range", line, startIndex));
	}
	}

	return true;
}

static bool	ExtractNumber(const std::string &getLine, 
const size_t line, 
size_t &i,
int32_t &pos, 
char delimiter)
{
	for (; isblank(getLine[i]); ++i);
	if (StringToNum(getLine, line, i, pos) == false)
	{
	return false;
	}
	for (; isblank(getLine[i]); ++i);
	if (getLine[i] != delimiter)
	{
	return Print::Err(CreateFileErrorMessage("Except '" + std::to_string(delimiter) + "'", line, i));
	}
	++i;
	return true;
}

const std::string nu = "";

RMD::RMD(): _fileName(nu), 
			_maxHeight(0), 
			_minHeight(0)
{
	this->_mapSize[X] = 0;
	this->_mapSize[Y] = 0;
	this->_mapSize[Z] = 0;
}
		
RMD::RMD(const std::string &fileName): _fileName(fileName), 
										_maxHeight(0), 
										_minHeight(0)
{
	this->_mapSize[X] = 0;
	this->_mapSize[Y] = 0;
	this->_mapSize[Z] = 0;
}

RMD::~RMD()
{

}

void	RMD::_InsertionAndSort(int32_t x, int32_t y, int32_t z)
{
	if (this->_specificPoints.size() == 0)
	{
		this->_specificPoints.push_back(Vec(x, y, z));
		return;
	}

	for (std::deque<Vec>::iterator	it = this->_specificPoints.begin(); it < this->_specificPoints.end(); ++it)
	{
		if (x < (it)->x)
		{
			this->_specificPoints.insert(it, Vec(x, y, z));
			return;
		}
		else if (x == (it)->x && y < (it)->y)
		{
			this->_specificPoints.insert(it, Vec(x, y, z));
			return;
		}
		else if (x == (it)->x && y == (it)->y)
		{
			return ;
		}
		else if (it == this->_specificPoints.end() - 1)
		{
			this->_specificPoints.insert(this->_specificPoints.end(), Vec(x, y, z));
		}
	}
}

bool	RMD::_StorePosition(const std::string &getLine, 
							const size_t line, 
							size_t &i)
{
	int32_t	x = 0;
	int32_t	y = 0;
	int32_t	z = 0;

	if (ExtractNumber(getLine, line, i, x, ',') == false)
	{
		return false;
	}
	// if (x == 0)
	// {
	// 	return true;
	// }
	if (ExtractNumber(getLine, line, i, y, ',') == false)
	{
		return false;
	}
	// if (y == 0)
	// {
	// 	return true;
	// }
	if (ExtractNumber(getLine, line, i, z, ')') == false)
	{
		return false;
	}

	if (this->_mapSize[X] < uint32_t(x))
	{
		this->_mapSize[X] = x;
	}
	if (this->_mapSize[Y] < uint32_t(y))
	{
		this->_mapSize[Y] = y;
	}
	if (this->_maxHeight < z)
	{
		this->_maxHeight = z;
	}
	else if (z < this->_minHeight)
	{
		this->_minHeight = z;
	}

	this->_InsertionAndSort(x, y, z);

	return true;
}

bool	RMD::_StoreVec(const std::string &getLine, const size_t line)
{
	size_t	getLineLen = getLine.length();

	for (size_t i = 0; i < getLineLen; ++i)
	{
		for (; isblank(getLine[i]); ++i);
		if (getLine[i] != '(')
		{
			return Print::Err(CreateFileErrorMessage("Except '('", line, i));
		}
		++i;
		if (this->_StorePosition(getLine, line, i) == false)
		{
			return false;
		}
	}

	return true;
}

bool	RMD::_ParseLines(std::ifstream &ifs)
{
	std::string	getLine;

	//std::istringstream iss;

	for (size_t line = 1; std::getline(ifs, getLine); ++line)
	{
		if (this->_StoreVec(getLine, line) == false)
		{
			return false;
		}
	}
	return true;
}

bool	RMD::_ParseModFile(void)
{
	std::ifstream	ifs;
	bool			errorFlg;

	ifs.open(this->_fileName);
	if (!ifs)
	{
		return Print::Err("Error: mod1 file: Opne failure");
	}
	errorFlg = this->_ParseLines(ifs);
	ifs.close();
	for (size_t i = 0; i < this->_specificPoints.size(); ++i)
	{
		std::cout << this->_specificPoints[i] << std::endl;
	}
	return errorFlg;
}

void	RMD::_DecideMapSize(void)
{
	this->_mapSize[X] += 1 + this->_mapSize[X] / 4;
	this->_mapSize[Y] += 1 + this->_mapSize[Y] / 4;
	this->_mapSize[Z] = this->_maxHeight;
	if (this->_minHeight < 0)
	{
		this->_mapSize[Z] += abs(this->_minHeight);
	}
	this->_mapSize[Z] += 1 + this->_maxHeight / 4;

	uint32_t	biggest = max(this->_mapSize[X], this->_mapSize[Y]);
	biggest = max(biggest, this->_mapSize[Z]);

	if (biggest == this->_mapSize[X])
	{
		assign_power_of_two(this->_mapSize[X], 
							this->_mapSize[Y], 
							this->_mapSize[Z]);
	}
	else if (biggest == this->_mapSize[Y])
	{
		assign_power_of_two(this->_mapSize[Y], 
							this->_mapSize[X], 
							this->_mapSize[Z]);
	}
	else if (biggest == this->_mapSize[Z])
	{
		assign_power_of_two(this->_mapSize[Z], 
							this->_mapSize[X], 
							this->_mapSize[Y]);
	}
}

void	RMD::_CalibrateHeight(void)
{
	if (0 <= this->_minHeight)
	{
		return ;
	}

	int64_t	calibrationHeight = abs(this->_minHeight);

	for (size_t	i = 0; i < this->_specificPoints.size(); ++i)
	{
		this->_specificPoints[i].z +=  calibrationHeight;
	}

}

void	RMD::_AddEndPoints(void)
{
	int64_t	calibrationHeight = 0;
	if (this->_minHeight <  calibrationHeight)
	{
		calibrationHeight = abs(this->_minHeight);
	}
	for (uint32_t x = 0; x < this->_mapSize[X]; ++x)
	{
		if (x == 0 || x == this->_mapSize[X] - 1)
		{
			for (uint32_t y = 0; y < this->_mapSize[Y]; ++y)
			{
				if (x == 0 && y == 0)
				{
					continue;
				}
				if (y % OMMIT == 0 || y + 1 ==  this->_mapSize[Y])
				{
					this->_specificPoints.push_back(Vec(x, y, calibrationHeight));
				}
			}
		}
		else if (x % OMMIT == 0)
		{
			this->_specificPoints.push_back(Vec(x, 0, calibrationHeight));
			this->_specificPoints.push_back(Vec(x, this->_mapSize[Y] - 1, calibrationHeight));
		}
	}
}

bool	RMD::ReadStart(void)
{
	if(!this->_ParseModFile())
	{
		return false;
	}
		
	this->_DecideMapSize();
	this->_CalibrateHeight();
	this->_AddEndPoints();
	if (this->_minHeight < 0)
	{
		this->_maxHeight += abs(this->_minHeight);
		this->_minHeight = 0;
	}

	return true;
}

std::deque<Vec>	RMD::GetSpecificPoints(void)
{
	return this->_specificPoints;
}

void	RMD::GetMapSize(uint32_t mapSize[3])
{
	mapSize[X] = this->_mapSize[X];
	mapSize[Y] = this->_mapSize[Y];
	mapSize[Z] = this->_mapSize[Z];

}

void	RMD::GetMaxMinHeight(int64_t &maxHeight, 
							 int64_t &minHeight)
{
	maxHeight = this->_maxHeight;
	minHeight = this->_minHeight;
}
