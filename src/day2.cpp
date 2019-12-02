#include "day2.h"

#include "util.h"

using std::pair;
using std::vector;
using std::string;

namespace day2
{

	class Intcode
	{
	private:
		vector<int> memory;
	public:

		Intcode(const vector<int>& memory): memory(memory)
		{ }

		void write(int address, int value)
		{
			memory[address] = value;
		}

		int read(int address)
		{
			return memory[address];
		}

		void run()
		{
			int ip = 0;

			while (true)
			{
				switch (memory[ip])
				{
				case 1:
					memory[memory[ip + 3]] = memory[memory[ip + 1]] + memory[memory[ip + 2]];
					break;
				case 2:
					memory[memory[ip + 3]] = memory[memory[ip + 1]] * memory[memory[ip + 2]];
					break;
				case 99:
					return;
				}

				ip += 4;
			}
		}
	};

	pair<int, int> solve(const vector<string>& input)
	{
		vector<int> memory = util::mapVector<string,int>(util::split(input[0], ','));

		Intcode computer(memory);
		computer.write(1, 12);
		computer.write(2, 2);
		computer.run();
		int part1 = computer.read(0);

		int part2 = 0;
		for (int noun = 0; noun < 100; noun++)
		{
			for (int verb = 0; verb < 100; verb++)
			{
				Intcode computer(memory);
				computer.write(1, noun);
				computer.write(2, verb);
				computer.run();

				if (computer.read(0) == 19690720)
				{
					part2 = 100 * noun + verb;
					goto done;
				}

			}
		} done:

		return pair<int, int>(part1, part2);
	}


}