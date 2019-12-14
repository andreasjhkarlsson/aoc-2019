
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include "util.h"
#include "days.h"

using std::cout;
using std::cin;
using std::getline;
using std::endl;
using std::string;
using std::vector;
using std::pair;

typedef pair<int64_t, int64_t>(*Solver)(const vector<string>&);

#define DAY(n) day##n::solve

const Solver solutions[] = {
	DAY(1),  DAY(2),  DAY(3),
	DAY(4),  DAY(5),  DAY(6),
	DAY(7),  DAY(8),  DAY(9),
	DAY(10), DAY(11)
};

int main()
{
	cout << "***************************" << endl;
	cout << "*** Advent of Code 2019 ***" << endl;
	cout << "***************************" << endl;

	std::cout << "Day: ";
	int day; cin >> day;

	cout << "Input: ";

	vector<string> input;
	string line;

	while (getline(cin, line))
	{
		util::trim(line);
		if (line.size() > 0)
			input.push_back(line);
	}
	
	if (day > 0 && day <= (sizeof solutions / sizeof *solutions))
	{
		auto result = solutions[day-1](input);
		cout << "Part 1: " << result.first << endl;
		cout << "Part 2: " << result.second << endl;
	}
	else
	{
		std::cout << "Day not solved" << std::endl;
		return 1;
	}
}

