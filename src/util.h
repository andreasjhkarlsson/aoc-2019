#pragma once

#include <string>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <functional>

namespace util
{

	// https://stackoverflow.com/a/17976541/242348
	inline std::string trim(const std::string &s)
	{
		auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) {return std::isspace(c); });
		return std::string(wsfront, std::find_if_not(s.rbegin(), std::string::const_reverse_iterator(wsfront), [](int c) {return std::isspace(c); }).base());
	}

	template <typename T>
	T convert(const std::string& str)
	{
		T result;
		std::istringstream(str) >> result;
		return result;
	}

	template <typename T, typename U>
	std::vector<U> mapVector(const std::vector<T>& vector, std::function<U(T)> fn = util::convert<U>)
	{
		std::vector<U> mapped(vector.size());
		std::transform(vector.begin(), vector.end(), mapped.begin(), fn);
		return mapped;
	}

	// https://stackoverflow.com/a/46931770/242348
	inline std::vector<std::string> split(const std::string &s, char delim)
	{
		std::vector<std::string> result;
		std::stringstream ss(s);
		std::string item;

		while (std::getline(ss, item, delim)) {
			result.push_back(item);
		}

		return result;
	}


}

