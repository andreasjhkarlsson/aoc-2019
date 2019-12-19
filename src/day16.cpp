#include "days.h"
#include "util.h"

using std::vector;

const vector<int> FFT_PATTERN = { 0, 1, 0, -1};

void phaseSecondHalf(const vector<int>& signal, vector<int>& out)
{
	int64_t sum = 0;
	for (int i = signal.size() - 1; i >= signal.size() / 2; i--)
	{
		sum += signal[i];
		out[i] = sum % 10;
	}
}

void phase(const vector<int>& signal, vector<int>& out)
{
	phaseSecondHalf(signal, out);
	for (int n = 0 ; n < signal.size() / 2;n++)
	{

		int sum = 0;
		for (int i = 0; i < signal.size(); i++)
		{
			sum += signal[i] * FFT_PATTERN[(i+1) / (n+1) % FFT_PATTERN.size()];
		}

		out[n] = abs(sum) % 10;
	}
}

template<void (*phaser)(const vector<int>&, vector<int>&), const int phases>
int64_t processSignal(const vector<int>& signal, int64_t resultOffset)
{
	vector<int> buffers[] = { signal, signal };

	for (int i = 0; i < 100; i++)
	{
		phaser(buffers[0], buffers[1]);
		std::swap(buffers[0], buffers[1]);
	}

	int64_t sum = 0;

	for (int i = 0; i < 8; i++)
		sum = sum * 10 + buffers[0][resultOffset+i];

	return sum;
}

day(16) = solve(const auto& input)
{
	vector<int> signal;
	
	for (int c : input[0])
		signal.push_back(c - '0');

	int64_t part1 = processSignal<phase, 100>(signal, 0);

	signal.clear();

	for (int i = 0; i < 10000; i++)
	{
		for (int c : input[0])
			signal.push_back(c - '0');
	}

	int64_t offset = 0;
	for (int i = 0; i < 7; i++)
		offset = offset * 10 + signal[i];

	int64_t part2 = processSignal<phaseSecondHalf, 100>(signal, offset);
	
	return { part1, part2 };
};