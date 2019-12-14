#pragma once
#include <utility>
#include <vector>
#include <string>
#include <functional>

typedef std::function<std::pair<int64_t, int64_t>(const std::vector<std::string>&)> Solver;

struct Day
{
	bool solved = false;
	Solver solver;
};

extern Day days[];

template<int D>
struct Solve
{
	Solve(Solver solver)
	{
		days[D - 1].solver = solver;
		days[D - 1].solved = true;
	}
};

#define day(n) Solve<n> solution

#define solve(p) (Solver) [](p) -> std::pair<int64_t,int64_t>