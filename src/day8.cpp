#pragma once

#include "days.h"
#include "util.h"
#include <array>
#include <iostream>

using std::pair;
using std::vector;
using std::string;
using std::array;
using std::cout;
using std::endl;

const int width = 25;
const int height = 6;
const int layerSize = width * height;

typedef char Layer[height][width];

day(8) = solve(const vector<string>& input)
{
	auto pixels = input[0];
	Layer* layers = (Layer*)pixels.c_str();
	auto layerCount = pixels.size() / layerSize;

	int part1 = 0;
	int lowestZeroes = 0x7FFFFFFF;

	for (int i = 0; i < layerCount; i++)
	{
		auto layer = layers[i];
		int counts[3] = { 0,0,0 };
		for (int p = 0; p < layerSize; p++)
		{
			auto pixel = ((char*)layer)[p];
			counts[pixel - '0']++;
		}

		if (counts[0] < lowestZeroes)
		{
			lowestZeroes = counts[0];
			part1 = counts[1] * counts[2];
		}

	}

	cout << endl;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			char out = ' ';
			for (int i = 0; i < layerCount; i++)
			{
				switch (layers[i][y][x])
				{
				case '1': out = '#';
				case '0': goto print;
				case '2': continue;
				}
			}

			print: cout << out;
		}

		cout << endl;
	}

	return { part1, 0 };

};