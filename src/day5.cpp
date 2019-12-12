#include "day5.h"
#include "util.h"
#include "intcode.h"

using std::pair;
using std::vector;
using std::string;

namespace day5
{

	int64_t runTEST(const vector<int64_t>& program, int input)
	{
		Intcode computer(program);
		computer.getInput().write(input);
		computer.run();

		int64_t code = 0;
		while (!computer.getOutput().empty())
			code = computer.getOutput().read().value();

		return code;
	}

	pair<int64_t, int64_t> solve(const vector<string>& input)
	{
		auto program = util::mapVector<string, int64_t>(util::split(input[0], ','));

		auto part1 = runTEST(program, 1);
		auto part2 = runTEST(program, 5);

		return pair(part1, part2);
	}

}