
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <optional>
#include "util.h"
#include "days.h"

using std::cout;
using std::cin;
using std::getline;
using std::endl;
using std::string;
using std::vector;
using std::pair;
using std::optional;

Day days[25];

optional<pair<int64_t,int64_t>> solveDay(Day& day, const vector<string>& input)
{
	if (day.solved)
		return day.solver(input);
	return std::nullopt;
}

int main()
{
	cout << "***************************" << endl;
	cout << "*** Advent of Code 2019 ***" << endl;
	cout << "***************************" << endl;

	cout << "Day: ";
	int day; cin >> day;

	if (day < 1 || day > 25)
	{
		cout << "Invalid day" << endl;
		return 1;
	}

	cout << "Input: ";

	vector<string> input;
	string line;

	while (getline(cin, line))
	{
		util::trim(line);
		if (line.size() > 0)
			input.push_back(line);
	}

	if (auto solution = solveDay(days[day - 1], input))
	{
		cout << "Part 1: " << solution.value().first << endl;
		cout << "Part 2: " << solution.value().second << endl;
	}
	else
	{
		cout << "Day not solved" << endl;
		return 1;
	}

}

