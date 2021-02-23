#pragma once
#include <string>
#include <vector>
#include <sstream>

inline std::vector< std::string> Split(const std::string& str, const std::string& delim)
{
	std::stringstream ss(str);
	std::string item;
	std::vector<std::string> splittedStrings;
	while (std::getline(ss, item, delim.at(0)))
	{
		splittedStrings.push_back(item);
	}
	return splittedStrings;
}
