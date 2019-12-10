#pragma once

#include "day6.h"
#include "util.h"
#include <map>
#include <optional>

using std::pair;
using std::vector;
using std::string;
using std::map;
using std::optional;

namespace day6
{
	int findDepth(const map<string, string>& orbits, const string& label, const string& target)
	{
		string next = orbits.at(label);
		if (next == target)
			return 1;
		else
			return 1 + findDepth(orbits, next, target);
	}

	void getPath(const map<string, string>& orbits, const string& label, const string& target, vector<string>& path)
	{
		string next = orbits.at(label);

		path.push_back(next);

		if (next != target)
			getPath(orbits, next, target, path);
	}

	pair<int, int> solve(const vector<string>& input)
	{
		auto columns = util::mapVector<string, pair<string, string>>(input, [](const auto& str) {
			auto splitted = util::split(str, ')');
			return pair(splitted[0], splitted[1]);
		});

		map<string, string> orbitals;
		for (auto orbit : columns)
			orbitals[orbit.second] = orbit.first;

		int sum = 0;
		for (auto& orbit : columns)
		{
			sum += findDepth(orbitals, orbit.second, "COM");
		}
		
		vector<string> myPath;
		getPath(orbitals, "YOU", "COM", myPath);

		vector<string> santasPath;
		getPath(orbitals, "SAN", "COM", santasPath);

		int distanceApart = 0;

		for (auto& step : myPath)
		{
			if (util::contains(santasPath, step))
			{
				distanceApart = findDepth(orbitals, "YOU", step) + findDepth(orbitals, "SAN", step) - 2;
				goto done;
			}
		}done:


		return pair(sum, distanceApart);
	}

}