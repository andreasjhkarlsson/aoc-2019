#include "days.h"
#include "util.h"

using std::pair;
using std::vector;
using std::string;

enum class Mode { Part1, Part2 };

bool matches(int digit, int rest, int repeats, int foundClusters, Mode mode)
{
	int nextDigit = rest % 10;

	if (digit < nextDigit)
		return false;

	if (digit == nextDigit)
		repeats++;
	else
		repeats = 1;

	if (repeats == 2)
		foundClusters++;

	if (mode == Mode::Part2 && repeats > 2)
		foundClusters--;

	if (rest >= 10)
		return matches(nextDigit, rest / 10, repeats, foundClusters, mode);
	else
		return foundClusters > 0;
}

bool matches(int number, Mode mode)
{
	return matches(number % 10, number / 10, 1, 0, mode);
}

day(4) = solve(const vector<string>& input)
{
	auto range = util::mapVector<string,int>(util::split(input[0], '-'));
	auto from = range[0];
	auto to = range[1];

	int part1Matches = 0;
	int part2Matches = 0;

	for (int password = from; password <= to; password++)
	{
		if (matches(password, Mode::Part1))
			part1Matches++;
		if (matches(password, Mode::Part2))
			part2Matches++;
	}

	return { part1Matches, part2Matches };

};