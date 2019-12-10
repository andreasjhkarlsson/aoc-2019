#pragma once

#include <vector>
#include <sstream>
#include <queue>
#include <optional>

enum Mode;
struct Parameter;

class Intcode
{
private:
	uint32_t ip;
	std::vector<int64_t> memory;
	std::queue<int64_t> input;
	std::queue<int64_t> output;
	int decodeParameter(uint32_t address, Parameter parameter);
public:
	Intcode(const std::vector<int64_t>& memory);

	void addInput(int64_t);

	std::optional<int64_t> readOutput();

	std::vector<int64_t> readAllOutput();

	int64_t& operator[](uint32_t address);

	void run();

	bool step();
};