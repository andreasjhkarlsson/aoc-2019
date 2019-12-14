#include "days.h"
#include "util.h"
#include "intcode.h"
#include <map>

using std::pair;
using std::vector;
using std::string;
using std::map;
using std::cout;
using std::endl;

enum Color { Black, White };

typedef map<util::Coord, Color> Hull;

class Robot
{
private:
	util::Coord position;
	util::Vector facing;

	class RobotEyes : public InputDevice<int64_t>
	{
		Robot& robot;
		map<util::Coord, Color>& hull;
	public:
		RobotEyes(Robot& robot, Hull& hull) : robot(robot), hull(hull)
		{ }
		virtual std::optional<int64_t> read() override
		{
			return hull[robot.position];
		}
	};

	class RobotArm : public OutputDevice<int64_t>
	{
		enum Mode { Paint, Move };
		Robot& robot;
		map<util::Coord, Color>& hull;
		Mode mode = Paint;

		void paint(Color color)
		{
			hull[robot.position] = color;
		}
		void move(int64_t direction)
		{
			switch (direction)
			{
			case 0:
				robot.facing = util::Vector{ robot.facing.y, -robot.facing.x };
				goto stepForward;
			case 1:
				robot.facing = util::Vector{ -robot.facing.y, robot.facing.x };
				goto stepForward;
			}
			
			stepForward: robot.position = robot.position + robot.facing;
		}

	public:
		RobotArm(Robot& robot, Hull& hull) : robot(robot), hull(hull)
		{ }

		virtual void write(const int64_t& value) override
		{
			switch (mode)
			{
			case Paint:
				paint((Color)value);
				break;
			case Move:
				move(value);
				break;
			}

			mode = (Mode)((mode + 1) % 2);
		}

		virtual void setEOF() override {}
	};

public:
	typedef IO<RobotEyes, RobotArm> IO;

	IO io;

	Robot(Hull& hull):
		position{0,0},
		facing{0,-1},
		io{RobotEyes(*this, hull), RobotArm(*this, hull)}
	{ }
};

void paintHull(Hull& hull, const vector<int64_t>& program)
{
	Robot robot(hull);

	Intcode<Robot::IO&> computer(program, robot.io);

	computer.run();
}

template <typename IT>
pair<int, int> findMinMax(IT begin, IT end, std::function<int(typename IT::value_type const & it)> fn)
{
	const auto [min, max] = std::minmax_element(begin, end, [&](const auto & left, const auto & right) {
		return fn(left) < fn(right);
	});

	return { fn(*min), fn(*max) };
}

day(11) = solve(const vector<string>& input)
{
	auto program = util::mapVector<string, int64_t>(util::split(input[0], ','));

	Hull hull;
	paintHull(hull, program);

	auto part1 = hull.size();

	hull.clear();
	hull[{0, 0}] = White;
	paintHull(hull, program);
		
	auto [minX, maxX] = findMinMax(hull.begin(), hull.end(), [](const auto & e) { return e.first.x; });
	auto [minY, maxY] = findMinMax(hull.begin(), hull.end(), [](const auto & e) { return e.first.y; });

	for (int y = minY; y <= maxY; y++)
	{
		for (int x = minX; x <= maxX; x++)
		{

			if (hull[{x, y}] == White)
				cout << '#';
			else
				cout << ' ';
		} cout << endl;
	} cout << endl;

	return { part1, 0 };
};