#include "days.h"
#include <map>
#include <optional>
#include <stdexcept>
#include <iostream>
#include "parser.h"

using std::pair;
using std::vector;
using std::string;
using std::map;
using std::optional;

const string ORE = "ORE";
const string FUEL = "FUEL";

struct Component
{
	int64_t amount;
	string name;
};

bool operator<(const Component& l, const Component& r)
{
	return pair{ l.amount, l.name } < pair{ r.amount, r.name };
}

namespace parser
{
	std::optional<Component> component(const char** input)
	{
		if (auto i = integer(input))
		{
			expect<' '>(input);
			return Component{ i.value(), expect<string,text>(input) };
		}

		return std::nullopt;
	}

	pair<Component, vector<Component>> reaction(const char* line)
	{
		vector<Component> inputs{ expect<Component, component>(&line) };

		while (accept<','>(&line))
		{
			expect<' '>(&line);
			inputs.push_back(expect<Component, component>(&line));
		}

		expect<' ', '=', '>', ' '>(&line);

		auto result = expect<Component, component>(&line);

		return { result, inputs };
	}
}

void synthesize(const string& chemical, int64_t amount, map<string,pair<Component, vector<Component>>>& recipies, map<string,int64_t>& created, map<string,int64_t>& used)
{
	int64_t leftover = created[chemical] - used[chemical];

	amount -= leftover;

	if (amount <= 0)
		return; // Leftovers were enough

	if (chemical == ORE)
	{
		created[ORE] += amount;
		return;
	}

	Component& component = recipies[chemical].first;
	vector<Component>& requirements = recipies[chemical].second;

	int64_t reactionsNeeded = (amount + component.amount - 1) / component.amount;

	for (const auto& requirement: requirements)
	{
		synthesize(requirement.name, requirement.amount * reactionsNeeded, recipies, created, used);
		used[requirement.name] += requirement.amount * reactionsNeeded;
	}

	created[component.name] += component.amount * reactionsNeeded;
}


int64_t requiredFor(const string& what, int64_t amount, const string& from, map<string, pair<Component, vector<Component>>>& recipies)
{
	map<string, int64_t> used;
	map<string, int64_t> created;
	synthesize(what, amount, recipies, created, used);
	return used[from];
}

int64_t binarySearch(const std::function<int64_t(int64_t)>& fn, int64_t from, int64_t to)
{
	int64_t val = (to-from) / 2 + from;

	int result = fn(val);

	if (result == 0)
		return val;
	else if (result == 1)
		return binarySearch(fn, val, to);
	else if (result == -1)
		return binarySearch(fn, from, val);
}



day(14) = solve(const auto & input)
{
	map<string,pair<Component, vector<Component>>> recipies;

	for (const auto& line : input)
	{
		auto r = parser::reaction(line.c_str());
		recipies[r.first.name] = r;
	}

	const int64_t part1 = requiredFor(FUEL, 1, ORE, recipies);

	const int64_t availableOre = 1000000000000;

	int64_t part2 = binarySearch([&](int64_t fuel) {
		
		int64_t o1 = requiredFor(FUEL, fuel, ORE, recipies);
		int64_t o2 = requiredFor(FUEL, fuel+1, ORE, recipies);

		if (o1 <= availableOre && o2 > availableOre)
			return 0; // Match!
		else if (o1 < availableOre)
			return 1;
		else if (o1 > availableOre)
			return -1;

	}, 0, availableOre);


	

	return { part1,part2 };
};
