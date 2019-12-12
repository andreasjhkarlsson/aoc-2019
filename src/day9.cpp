#pragma once

#include "day9.h"
#include "util.h"
#include "intcode.h"

using std::pair;
using std::vector;
using std::string;

namespace day9
{

	pair<int64_t, int64_t> solve(const vector<string>& input)
	{
		auto program = util::mapVector<string, int64_t>(util::split(input[0], ','));

		Intcode computer(program);
		computer.getInput().write(1);
		computer.run();
		auto part1 = computer.getOutput().read().value();

		auto computer2 = Intcode(program);
		computer2.getInput().write(2);
		computer2.run();
		auto part2 = computer2.getOutput().read().value();

		return pair(part1, part2);

	}

}