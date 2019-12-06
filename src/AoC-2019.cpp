
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include "util.h"
#include "day1.h"
#include "day2.h"
#include "day3.h"
#include "day4.h"
#include "day5.h"
#include "day6.h"

using std::cout;
using std::cin;
using std::getline;
using std::endl;
using std::string;
using std::vector;
using std::pair;

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
	pair<int, int> result;

	switch (day)
	{
	case 1:
		result = day1::solve(input);
		break;
	case 2:
		result = day2::solve(input);
		break;
	case 3:
		result = day3::solve(input);
		break;
	case 4:
		result = day4::solve(input);
		break;
	case 5:
		result = day5::solve(input);
		break;
	case 6:
		result = day6::solve(input);
		break;
	default:
		cout << "Day not solved." << endl;
		return 1;
	}

	cout << "Part 1: " << result.first << endl;
	cout << "Part 2: " << result.second << endl;

}

