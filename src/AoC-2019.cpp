
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "util.h"
#include "day1.h"

using std::cout;
using std::cin;
using std::getline;
using std::endl;
using std::string;
using std::vector;
using std::pair;

template<typename T1, typename T2>
void printResult(pair<T1, T2> result)
{
	cout << "Part 1: " << result.first << endl;
	cout << "Part 2: " << result.second << endl;
}

int main()
{
    
	cout << "****************************" << endl;
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
		


	switch (day)
	{
	case 1:
		cout << "Solving day 1..." << endl;
		printResult(day1::solve(input));
		break;
	default:
		cout << "Day not solved." << endl;
		return 1;

	}

}

