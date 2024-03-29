#include "days.h"
#include "util.h"
#include "intcode.h"

using std::pair;
using std::vector;
using std::string;



day(9) = solve(const vector<string>& input)
{
	auto program = util::mapVector<string, int64_t>(util::split(input[0], ','));

	Intcode computer(program);
	computer << 1;
	computer.run();
	int64_t part1;
	computer >> part1;

	auto computer2 = Intcode(program);
	computer2 << 2;
	computer2.run();
	int64_t part2;
	computer >> part2;

	return { part1, part2 };

};