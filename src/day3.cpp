#include "days.h"
#include "util.h"
#include <set>
#include <map>

using std::pair;
using std::vector;
using std::string;
using std::istringstream;
using std::map;
using std::set;

struct Instruction
{
	char direction;
	short distance;
};

Instruction parseInstruction(const string& str)
{
	Instruction inst;

	istringstream stream(str);
	stream >> inst.direction;
	stream >> inst.distance;

	return inst;
}

vector<Instruction> parseWire(const string& str)
{
	return util::mapVector<string, Instruction>(util::split(str, ','), parseInstruction);
}

pair<set<util::Coord>,map<util::Coord, int>> wirePath(const vector<Instruction>& wire)
{
	util::Coord pos(0, 0);
	set<util::Coord> path;
	map<util::Coord, int> distances;
	int totalDistance = 0;

	for (const auto& inst : wire)
	{
		for (int i = 0; i < inst.distance; i++)
		{
			totalDistance++;

			switch (inst.direction)
			{
			case 'U':
				pos.y++;
				break;
			case 'D':
				pos.y--;
				break;
			case 'R':
				pos.x++;
				break;
			case 'L':
				pos.x--;
				break;
			}
			path.insert(pos);

			if (distances.find(pos) == distances.end())
				distances[pos] = totalDistance;
		}
	}

	return pair(path, distances);
}

int manhattanDistance(util::Coord c1, util::Coord c2)
{
	return abs(c1.x - c2.x) + abs(c1.y - c2.y);
}

day(3) = solve(const vector<string>& input)
{
	auto wires = util::mapVector<string, vector<Instruction>>(input, parseWire);

	auto wire0 = wirePath(wires[0]);
	auto wire1 = wirePath(wires[1]);

	std::vector<util::Coord> intersections;

	std::set_intersection(wire0.first.begin(), wire0.first.end(), wire1.first.begin(), wire1.first.end(), std::back_inserter(intersections));

	auto distances = util::mapVector<util::Coord, int>(intersections, [](const auto& c) {
		return manhattanDistance(util::Coord(0, 0), c);
	});

	std::sort(distances.begin(), distances.end());
	auto part1 = distances[0];

	distances = util::mapVector<util::Coord, int>(intersections, [&](const auto& c) {
		return wire0.second[c] + wire1.second[c];
	});

	std::sort(distances.begin(), distances.end());
	auto part2 = distances[0];

	return { part1, part2 };
};

