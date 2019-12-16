#include "days.h"
#include <regex>
#include "util.h"
#include <cmath>
#include <set>
#include <tuple>

using std::vector;
using std::string;
using std::set;
using std::pair;
using std::tuple;

const std::regex inputPattern( "\\=(-*\\d+)");

struct Vector3
{
	int x;
	int y;
	int z;

	Vector3 operator+(const Vector3& other) const
	{
		return { this->x + other.x, this->y + other.y, this->z + other.z };
	}

	Vector3 operator-(const Vector3& other) const
	{
		return { this->x - other.x, this->y - other.y, this->z - other.z };
	}
	Vector3& operator+=(const Vector3& other)
	{
		return *this = *this + other;
	}

	Vector3& operator-=(const Vector3& other)
	{
		return *this = *this - other;
	}
};

struct Moon
{
	Vector3 position;
	Vector3 velocity;

	int energy()
	{
		return
			(abs(position.x) + abs(position.y) + abs(position.z)) *
			(abs(velocity.x) + abs(velocity.y) + abs(velocity.z));
	}
};

int calculateGravity(Moon& m1, Moon& m2, std::function<int(const Vector3&)> axis)
{
	if (axis(m1.position) < axis(m2.position)) return 1;
	else if (axis(m1.position) > axis(m2.position)) return -1;
	else return 0;
};

void step(vector<Moon>& moons)
{
	
	for (int i = 0; i < moons.size(); i++)
	{
		for (int j = i+1; j < moons.size(); j++)
		{
			auto[m1, m2] = std::pair {&moons[i], &moons[j]};

			Vector3 dv = {
				calculateGravity(*m1, *m2, [](const auto& m) { return m.x; }),
				calculateGravity(*m1, *m2, [](const auto& m) { return m.y; }),
				calculateGravity(*m1, *m2, [](const auto& m) { return m.z; })
			};

			m1->velocity += dv;
			m2->velocity -= dv;
		}
	}

	for (int i = 0; i < moons.size(); i++)
		moons[i].position += moons[i].velocity;
}

int findRepeat(vector<Moon> moons, Vector3 Moon::* vec, int Vector3::* component)
{
	vector<int> initialStates;
	for (auto& moon : moons)
		initialStates.push_back(moon.*vec.*component);

	for (int n = 1;; n++)
	{

		step(moons);
		vector<int> states;
		for (auto& moon : moons)
			states.push_back(moon.*vec.*component);

		if (initialStates == states)
			return n;
	}
}

int64_t gcd(int64_t a, int64_t b)
{
	for (;;)
	{
		if (a == 0) return b;
		b %= a;
		if (b == 0) return a;
		a %= b;
	}
}

int64_t lcm(int64_t a, int64_t b)
{
	int64_t temp = gcd(a, b);

	return temp ? (a / temp * b) : 0;
}

int64_t lcm(int64_t* arr, int64_t count)
{
	if (count == 2)
		return lcm(arr[0], arr[1]);
	else
		return lcm(arr[0], lcm(arr + 1, count - 1));
}

day(12) = solve(const auto& input)
{
	
	auto moons = util::mapVector<string, Moon>(input, [](const string& line) -> Moon {

		std::sregex_iterator iter(line.begin(), line.end(), inputPattern);
		
		return {
			util::convert<int>((*iter++)[1]),
			util::convert<int>((*iter++)[1]),
			util::convert<int>((*iter)[1]),
		};
	});

	// Do part 2 before part 1 since part 1 is destructive
	int64_t cycles[] = {
		findRepeat(moons, &Moon::position, &Vector3::x) + 1, // ?
		findRepeat(moons, &Moon::position, &Vector3::y) + 1, // ?????
		findRepeat(moons, &Moon::position, &Vector3::z) + 1, // ??????????????
		findRepeat(moons, &Moon::velocity, &Vector3::x),
		findRepeat(moons, &Moon::velocity, &Vector3::y),
		findRepeat(moons, &Moon::velocity, &Vector3::z)
	};

	int64_t part2 = lcm(cycles, 6);

	int steps = 1000;
	while (steps --> 0) step(moons);

	int64_t part1 = 0;
	for (auto& moon : moons)
		part1 += moon.energy();

	return { part1, part2 };
};