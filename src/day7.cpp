#pragma once

#include "day7.h"
#include "util.h"
#include "intcode.h"
#include <cmath>

using std::pair;
using std::vector;
using std::string;

namespace day7
{
	int runAmplifiers(Intcode* amplifiers, int count, int input)
	{
		amplifiers[0].addInput(input);
		amplifiers[0].run();

		auto output = amplifiers[0].readOutput().value();
		
		if (count > 1)
			return runAmplifiers(amplifiers + 1, count - 1, output);
		else
			return output;
	}

	int splitPhase(int n, int f)
	{
		if (f > 0)
			return splitPhase(n / 5, f - 1);
		else
			return n % 5;
	}

	void setPhases(Intcode* amplifiers, int* phases, int count)
	{
		if (count > 0)
		{
			amplifiers[0].addInput(phases[0]);
			setPhases(amplifiers + 1, phases + 1, count - 1);
		}
	}


	int runAmplifierLoop(Intcode* amplifiers, int count, int input)
	{
		int finalOutput = 0;
		auto& lastAmplifier = amplifiers[count - 1];

		for (int i = 0;; i++) 
		{
			auto& amplifier = amplifiers[i%count];
			amplifier.addInput(input);
			while (amplifier.step())
			{
				auto output = amplifier.readOutput();
				if (output.has_value())
				{
					input = output.value();
					if (&amplifier == &lastAmplifier) // always save result of last amplifier
						finalOutput = input;
					goto next; // love it
				}
			}

			return finalOutput;
			next:;

		}
	}

	pair<int64_t, int64_t> solve(const vector<string>& input)
	{
		auto program = util::mapVector<string, int64_t>(util::split(input[0], ','));

		vector<Intcode> amplifiers(5, Intcode(program));

		int maxOutput = 0;
		auto phaseSettings = util::permuteUnique({ 0, 1, 2, 3, 4 });

		for (auto& phases: phaseSettings)
		{
			vector<Intcode> amplifiers(5, Intcode(program));

			setPhases(&amplifiers[0], &phases[0], 5);

			int output = runAmplifiers(&amplifiers[0], 5, 0);

			if (output > maxOutput)
				maxOutput = output;
		}

		int part1 = maxOutput;

		maxOutput = 0;
		phaseSettings = util::permuteUnique({ 5,6,7,8,9 });

		int c = 0;
		for (auto& phases : phaseSettings)
		{
			std::cout << c++ << std::endl;
			vector<Intcode> amplifiers(5, Intcode(program));

			setPhases(&amplifiers[0], &phases[0], 5);

			int output = runAmplifierLoop(&amplifiers[0], 5, 0);

			if (output > maxOutput)
				maxOutput = output;
		}

		int part2 = maxOutput;

		return pair(part1, part2);

	}

}