#include "day2.h"
#include "intcode.h"
#include "util.h"

using std::pair;
using std::vector;
using std::string;

namespace day2
{

	pair<int64_t, int64_t> solve(const vector<string>& input)
	{
		vector<int64_t> memory = util::mapVector<string,int64_t>(util::split(input[0], ','));

		Intcode computer(memory);
		computer.getMemory()[1] = 12;
		computer.getMemory()[2] = 2;
		computer.run();
		int64_t part1 = computer.getMemory()[0];

		int part2 = 0;
		for (int noun = 0; noun < 100; noun++)
		{
			for (int verb = 0; verb < 100; verb++)
			{
				Intcode computer(memory);
				computer.getMemory()[1] = noun;
				computer.getMemory()[2] = verb;
				computer.run();

				if (computer.getMemory()[0] == 19690720)
				{
					part2 = 100 * noun + verb;
					goto done;
				}

			}
		} done:

		return pair<int64_t, int64_t>(part1, part2);
	}


}