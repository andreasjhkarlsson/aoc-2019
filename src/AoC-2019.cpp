
#include <istream>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <optional>
#include <fstream>
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

void readInput(std::istream& iStream, vector<string>& input)
{
	for (string line; getline(iStream, line);)
	{
		util::trim(line);
		if (line.size() > 0)
			input.push_back(line);
	}
}

int main(int argc, char* argv[])
{
	int day = 0;
	vector<string> input;

	for (int a = 1; a < argc; a++)
	{
		if (argv[a] == string("-d") && (a + 1) < argc)
		{
			day = util::convert<int>(string(argv[a + 1]));
		}
		else if (argv[a] == string("-i") && (a + 1) < argc)
		{
			std::ifstream ifs(argv[a+1]);
			readInput(ifs, input);
		}

	}

	cout << "***************************" << endl;
	cout << "*** Advent of Code 2019 ***" << endl;
	cout << "***************************" << endl;

	if (day == 0)
	{
		cout << "Day: "; cin >> day;
	}

	if (day < 1 || day > 25)
	{
		cout << "Invalid day" << endl;
		return 1;
	}

	if (input.size() == 0)
	{

		cout << "Input: ";
		readInput(cin, input);
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

