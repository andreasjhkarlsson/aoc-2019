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
	int64_t runAmplifiers(Intcode* amplifiers, int count, int64_t input)
	{
		amplifiers[0].getInput().write(input);
		amplifiers[0].run();

		auto output = amplifiers[0].getOutput().read();
		
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
			amplifiers[0].getInput().write(phases[0]);
			setPhases(amplifiers + 1, phases + 1, count - 1);
		}
	}


	int64_t runAmplifierLoop(Intcode* amplifiers, int count, int64_t input)
	{
		for (int i = 0; i < count; i++)
			amplifiers[i].run(false);
		
		int64_t finalOutput = 0;
		auto& lastAmplifier = amplifiers[count - 1];

		for (int i = 0;; i++) 
		{
			auto& amplifier = amplifiers[i%count];
			amplifier.getInput().write(input);

			bool eof = false;
			auto output = amplifier.getOutput().read(eof);
			
			if (eof)
				return finalOutput;

			if (&amplifier == &lastAmplifier) // always save result of last amplifier
				finalOutput = output;

			input = output;
		}
		
	}

	pair<int64_t, int64_t> solve(const vector<string>& input)
	{
		auto program = util::mapVector<string, int64_t>(util::split(input[0], ','));

		vector<Intcode> amplifiers(5, Intcode(program));

		int64_t maxOutput = 0;
		auto phaseSettings = util::permuteUnique({ 0, 1, 2, 3, 4 });

		for (auto& phases: phaseSettings)
		{
			vector<Intcode> amplifiers(5, Intcode(program));

			setPhases(&amplifiers[0], &phases[0], 5);

			auto output = runAmplifiers(&amplifiers[0], 5, 0);

			if (output > maxOutput)
				maxOutput = output;
		}

		int64_t part1 = maxOutput;

		maxOutput = 0;
		phaseSettings = util::permuteUnique({ 5,6,7,8,9 });

		for (auto& phases : phaseSettings)
		{
			vector<Intcode> amplifiers(5, Intcode(program));

			setPhases(&amplifiers[0], &phases[0], 5);

			int64_t output = runAmplifierLoop(&amplifiers[0], 5, 0);

			if (output > maxOutput)
				maxOutput = output;
		}

		int64_t part2 = maxOutput;

		return pair(part1, part2);

	}

}