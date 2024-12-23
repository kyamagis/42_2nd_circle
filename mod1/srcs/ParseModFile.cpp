#include "../includes/Print.hpp"
#include "../includes/Coordinates.hpp"
#include "../includes/ParseModFile.hpp"

#include <fstream>
#include <string>
#include <ctype.h>


#define ERR_EXCEPT "Error: mod1 file: Except "

std::string	CreateFileErrorMessage(const std::string &errorStr,
								 const size_t line, 
								 size_t i)
{

	return  "Error: mod1 file: " + errorStr
			+ ": line " + std::to_string(line) 
			+ ": char " + std::to_string(++i);
}

bool	StringToNum(const std::string &getLine, 
					const size_t line, 
					size_t &i, 
					int32_t &pos)
{
	size_t		startIndex = i;

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

bool	StorePosition(const std::string &getLine, 
					  std::deque<Coordinates> &posDeque, 
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
	if (ExtractNumber(getLine, line, i, y, ',') == false)
	{
		return false;
	}
	if (ExtractNumber(getLine, line, i, z, ')') == false)
	{
		return false;
	}
	posDeque.push_back(Coordinates(x, y, z));

	return true;
}

bool	StoreCoordinates(std::deque<Coordinates> &posDeque, 
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
		if (StorePosition(getLine, posDeque, line, i) == false)
		{
			return false;
		}
	}

	return true;
}

bool	ParseLines(std::ifstream &ifs, std::deque<Coordinates> &posDeque)
{
	std::string	getLine;

	//std::istringstream iss;

	for (size_t line = 1; std::getline(ifs, getLine); ++line)
	{
		if (StoreCoordinates(posDeque, getLine, line) == false)
		{
			return false;
		}
	}
	return true;
}

bool	ParseModFile(const std::string &fileName, std::deque<Coordinates> &posDeque)
{
	std::ifstream	ifs;
	bool			errorFlg;

	ifs.open(fileName);
	if (!ifs)
	{
		return Print::Err("Error: mod1 file: Opne failure");
	}
	errorFlg = ParseLines(ifs, posDeque);
	ifs.close();
	return errorFlg;
}