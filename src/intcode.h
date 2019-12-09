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
	int ip;
	std::vector<int> memory;
	std::queue<int> input;
	std::queue<int> output;
	int decodeParameter(int address, Parameter parameter);
public:
	Intcode(const std::vector<int>& memory);

	void writeMemory(int address, int value);

	int readMemory(int address);

	void addInput(int);

	std::optional<int> readOutput();

	std::vector<int> readAllOutput();

	void run();

	bool step();
};