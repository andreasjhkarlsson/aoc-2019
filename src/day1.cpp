#include "days.h"
#include "util.h"
#include <algorithm>

using std::pair;
using std::vector;
using std::string;


int fuelForMass(int mass)
{
	return mass / 3 - 2;
}

int fuelForModule(int _module)
{
	int fuel = fuelForMass(_module);

	if (fuel <= 0)
		return 0;

	return fuel + fuelForModule(fuel); // Treat fuel as new module
}

day(1) = solve(const auto& input)
{
	auto modules = util::mapVector<string, int>(input);

	int fuelPart1 = 0;
	int fuelPart2 = 0;

	for (int _module : modules)
	{
		fuelPart1 += fuelForMass(_module);
		fuelPart2 += fuelForModule(_module);
	}

	return { fuelPart1, fuelPart2 };
};