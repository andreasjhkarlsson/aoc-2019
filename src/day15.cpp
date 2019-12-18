#include "intcode.h"
#include "days.h"
#include "util.h"
#include <initializer_list>
#include <map>
#include <set>
#include <vector>

using std::set;
using std::vector;

enum Direction { North = 1, South = 2, West = 3, East = 4};

Direction operator-(Direction dir)
{
	switch (dir)
	{
	case North: return South;
	case South: return North;
	case West: return East;
	case East: return West;
	}
}

util::Coord operator+(util::Coord c,Direction dir)
{
	switch (dir)
	{
	case North: return { c.x, c.y - 1 };
	case South: return { c.x, c.y + 1 };
	case West: return { c.x - 1, c.y };
	case East: return { c.x + 1, c.y };
	}
}

struct Node
{
	bool oxygenSystem = false;
	bool hasOxygen = false;
	vector<Node> nodes;
};

class RepairDroid : public Intcode<BufferedIO>
{
public:
	enum Status { HitWall = 0, Moved = 1, MovedToOxygen = 2 };
	using Intcode::Intcode;

	Status move(Direction dir)
	{
		(*this) << (int64_t)dir;
		int64_t status;
		(*this) >> status;
		return (Status)status;
	}
};

Node explore(RepairDroid& robot, set<util::Coord>& visited, util::Coord position)
{
	Node node;
	for (auto dir : { North, South, West, East })
	{
		if (visited.contains(position + dir))
			goto neeeeeext;
		visited.insert(position + dir);

		switch (robot.move(dir))
		{
		case RepairDroid::Moved:
			node.nodes.push_back(explore(robot, visited, position + dir));
			goto backtrack;
		case RepairDroid::MovedToOxygen:
			node.nodes.push_back({ true, false, {} });
			goto backtrack;
		case RepairDroid::HitWall:
			goto neeeeeext;
		default: backtrack:
			robot.move(-dir);
		}
		neeeeeext:;
	}

	return node;
}

std::optional<int64_t> oxygenDepth(const Node& maze)
{
	if (maze.oxygenSystem)
		return 0;

	for (const auto& node : maze.nodes)
	{
		if (auto depth = oxygenDepth(node))
			return 1 + depth.value();
	}
	return std::nullopt;
}

bool fillOxygen(Node& maze, bool parentHadOxygen = false)
{
	bool hadOxygenBefore = maze.hasOxygen;

	if (maze.oxygenSystem || parentHadOxygen)
		maze.hasOxygen = true;

	for (auto& node: maze.nodes)
	{
		bool childHadOxygen = fillOxygen(node, hadOxygenBefore);
		if (childHadOxygen)
			maze.hasOxygen = true;
	}

	return hadOxygenBefore;
}

int64_t countNodes(const Node& maze, const std::function<bool(const Node&)>& fn = [](const Node& node) { return true; })
{
	int count = fn(maze) ? 1: 0;
	for (const auto& node : maze.nodes)
		count += countNodes(node, fn);

	return count;
}

#define countOygenatedNodes(nodes) countNodes(nodes, [](const Node& node) { return node.hasOxygen; })

day(15) = solve(const auto & input)
{
	RepairDroid droid(input[0]);
	droid.run(false);

	set<util::Coord> visited;
	Node maze = explore(droid, visited, { 0,0 });
	droid << eof; // Shut off droid (poor fella will panic)

	auto part1 = oxygenDepth(maze).value();

	int minutes;
	for (minutes = 0;countOygenatedNodes(maze) != countNodes(maze);minutes++)
		fillOxygen(maze);

	auto part2 = minutes - 1; // ????????

	return { part1, part2 };
};