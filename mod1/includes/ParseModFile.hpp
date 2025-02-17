#ifndef PARSEMODFILE_HPP
# define PARSEMODFILE_HPP

#include <deque>
bool	ParseModFile(const std::string &fileName, std::deque<Vec> &specificPoints, 
                     int64_t &maxHeight, int64_t &minHeight);

#endif