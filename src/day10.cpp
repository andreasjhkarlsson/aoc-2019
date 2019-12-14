#include "days.h"
#include "util.h"
#include <cmath>
#include <map>
#include <set>

using std::pair;
using std::vector;
using std::string;
using std::map;
using std::set;

const double PI = 3.14159; // Defining your own pi is honest work


double angleTo(util::Coord origo, util::Coord target)
{
	double a = -target.y - -origo.y;
	double b = -(target.x - origo.x);
	double c = sqrt(a * a + b * b);

	double angle = acos(b / c) + PI / 2.0;

	if (a < 0)
		angle += PI;

	if (angle >= PI * 2.0)
		angle -= PI * 2.0;

	return angle;
}

uint64_t intify(double angle)
{
	return (int)(angle * 100000000);
}

double distance(util::Coord p1, util::Coord p2)
{
	double a = p2.y - p1.y;
	double b = p2.x - p1.x;
	return sqrt(a * a + b * b);
}

day(10) = solve(const vector<string>& input)
{
	set<util::Coord> asteroids;
	for (int y = 0;y<input.size();y++)
	{
		for (int x = 0;x<input[y].size();x++)
		{
			if (input[y][x] == '#')
				asteroids.insert(util::Coord(x, y));
		} 
	} 

	int64_t part1 = 0;
	util::Coord station { 0, 0 };

	for (const util::Coord& asteroid: asteroids)
	{
		set<uint64_t> uniqueAngles;
		for (const util::Coord& target : asteroids)
		{
			if (&target == &asteroid) continue;
				
			double angle = angleTo(asteroid, target);
			uniqueAngles.insert(intify(angle)); // 4 decimals of precision should be enough

		}

		int64_t canSee = uniqueAngles.size();

		if (canSee > part1)
		{
			part1 = canSee;
			station = asteroid;
		}
	}

	asteroids.erase(station);
	map<uint64_t, set<util::Coord>> traced;
	for (const util::Coord& target : asteroids)
		traced[intify(angleTo(station, target) - PI)].insert(target);
		
	util::Coord lastKill { 0,0 };
	int kills = 0;
	sweep:
	for (auto & [_, targets] : traced)
	{
		if (targets.empty())
			continue;

		util::Coord toKill = *targets.begin();

		for (const util::Coord& candidate : targets)
		{
			if (distance(station, candidate) < distance(station, toKill))
				toKill = candidate;
		}

		targets.erase(toKill);
		lastKill = toKill;

		if (++kills == 200)
			goto done;
	} goto sweep;


	done:
	int part2 = lastKill.x * 100 + lastKill.y;

	return { part1, part2 };
};