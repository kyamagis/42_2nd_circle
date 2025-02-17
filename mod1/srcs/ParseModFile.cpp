#include "../includes/Print.hpp"
#include "../includes/Vec.hpp"
#include "../includes/ParseModFile.hpp"

#include <fstream>
#include <string>
#include <ctype.h>

std::string	CreateFileErrorMessage(const std::string &errorStr,
								 const size_t line, 
								 size_t i)
{

	return  "Error: mod1 file: " + errorStr
			+ ": line " + std::to_string(line) 
			+ ": column " + std::to_string(++i);
}

bool	StringToNum(const std::string &getLine, 
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

bool	ExtractNumber(const std::string &getLine, 
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

void	insertionAndSort(std::deque<Vec> &specificPoints, 
					  int32_t x, 
					  int32_t y,
					  int32_t z)
{
	if (specificPoints.size() == 0)
	{
		specificPoints.push_back(Vec(x, y, z));
		return;
	}

	for (std::deque<Vec>::iterator	it = specificPoints.begin(); it < specificPoints.end(); ++it)
	{
		if (x < (it)->x)
		{
			specificPoints.insert(it, Vec(x, y, z));
			return;
		}
		else if (x == (it)->x && y < (it)->y)
		{
			specificPoints.insert(it, Vec(x, y, z));
			return;
		}
		else if (x == (it)->x && y == (it)->y)
		{
			return ;
		}
		else if (it == specificPoints.end() - 1)
		{
			specificPoints.insert(specificPoints.end(), Vec(x, y, z));
		}
	}
}

bool	StorePosition(const std::string &getLine, 
					  std::deque<Vec> &specificPoints, 
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
	if (x == 0)
	{
		return true;
	}
	if (ExtractNumber(getLine, line, i, y, ',') == false)
	{
		return false;
	}
	if (y == 0)
	{
		return true;
	}
	if (ExtractNumber(getLine, line, i, z, ')') == false)
	{
		return false;
	}
	// specificPoints.push_back(Vec(x, y, z));
	insertionAndSort(specificPoints, x, y, z);

	return true;
}

bool	StoreVec(std::deque<Vec> &specificPoints, 
						 const std::string &getLine, 
						 const size_t line)
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
		if (StorePosition(getLine, specificPoints, line, i) == false)
		{
			return false;
		}
	}

	return true;
}

bool	ParseLines(std::ifstream &ifs, std::deque<Vec> &specificPoints)
{
	std::string	getLine;

	//std::istringstream iss;

	for (size_t line = 1; std::getline(ifs, getLine); ++line)
	{
		if (StoreVec(specificPoints, getLine, line) == false)
		{
			return false;
		}
	}
	return true;
}

bool	ParseModFile(const std::string &fileName, std::deque<Vec> &specificPoints)
{
	std::ifstream	ifs;
	bool			errorFlg;

	ifs.open(fileName);
	if (!ifs)
	{
		return Print::Err("Error: mod1 file: Opne failure");
	}
	errorFlg = ParseLines(ifs, specificPoints);
	ifs.close();
	for (size_t i = 0; i < specificPoints.size(); ++i)
	{
		std::cout << specificPoints[i] << std::endl;
	}
	return errorFlg;
}