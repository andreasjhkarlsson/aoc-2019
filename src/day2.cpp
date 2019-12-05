#include "day2.h"
#include "intcode.h"
#include "util.h"

using std::pair;
using std::vector;
using std::string;

namespace day2
{

	pair<int, int> solve(const vector<string>& input)
	{
		vector<int> memory = util::mapVector<string,int>(util::split(input[0], ','));

		Intcode computer(memory);
		computer.writeMemory(1, 12);
		computer.writeMemory(2, 2);
		computer.run();
		int part1 = computer.readMemory(0);

		int part2 = 0;
		for (int noun = 0; noun < 100; noun++)
		{
			for (int verb = 0; verb < 100; verb++)
			{
				Intcode computer(memory);
				computer.writeMemory(1, noun);
				computer.writeMemory(2, verb);
				computer.run();

				if (computer.readMemory(0) == 19690720)
				{
					part2 = 100 * noun + verb;
					goto done;
				}

			}
		} done:

		return pair<int, int>(part1, part2);
	}


}