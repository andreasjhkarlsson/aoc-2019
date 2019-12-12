#include "day5.h"
#include "util.h"
#include "intcode.h"

using std::pair;
using std::vector;
using std::string;

namespace day5
{

	int64_t runTEST(const vector<int64_t>& program, int64_t input)
	{
		Intcode computer(program);
		computer << input;
		computer.run();


		vector<int64_t> output;
		computer.getOutputDevice().readAll(output);

		return output[output.size() - 1];
	}

	pair<int64_t, int64_t> solve(const vector<string>& input)
	{
		auto program = util::mapVector<string, int64_t>(util::split(input[0], ','));

		auto part1 = runTEST(program, 1);
		auto part2 = runTEST(program, 5);

		return pair(part1, part2);
	}

}