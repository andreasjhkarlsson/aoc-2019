#pragma once

#include <string>
#include <cctype>
#include <algorithm>
#include <iostream>
namespace util
{

	// https://stackoverflow.com/a/17976541/242348
	inline std::string trim(const std::string &s)
	{
		auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) {return std::isspace(c); });
		return std::string(wsfront, std::find_if_not(s.rbegin(), std::string::const_reverse_iterator(wsfront), [](int c) {return std::isspace(c); }).base());
	}

	inline int stringToInt(std::string str)
	{
		trim(str);
		return std::stoi(str);
	}
}

