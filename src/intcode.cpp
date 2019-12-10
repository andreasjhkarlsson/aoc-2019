#include "intcode.h"

using std::queue;
using std::vector;
using std::optional;

enum Mode
{
	Position = 0,
	Immediate = 1
};

struct Parameter
{
	int offset;
	Mode mode;
	Parameter(int offset, Mode mode): offset(offset), mode(mode)
	{ }
};

class Opcode
{
private:
	vector<Mode> parameterModes;
public:
	const int op;
	Opcode(int code) : op(code % 100)
	{
		code /= 100;
		while (code > 0)
		{
			parameterModes.push_back((Mode)(code % 10));
			code /= 10;
		}
	}

	Parameter getParameter(int parameter)
	{
		return Parameter(parameter, parameter <= parameterModes.size() ? parameterModes[parameter-1] : Mode::Position);
	}
};


Intcode::Intcode(const vector<int64_t>& memory) : ip(0), memory(memory)
{ }

int64_t& Intcode::operator[](uint32_t address)
{
	if (address >= memory.size())
		memory.resize(address + 1);

	return memory[address];
}

void Intcode::addInput(int64_t value)
{
	input.push(value);
}

optional<int64_t> Intcode::readOutput()
{
	if (output.size() == 0)
		return std::nullopt;
	auto value = output.front();
	output.pop();
	return value;
}

vector<int64_t> Intcode::readAllOutput()
{
	vector<int64_t> all;

	while (!output.empty())
	{
		all.push_back(output.front());
		output.pop();
	}

	return all;
}

int Intcode::decodeParameter(uint32_t address, Parameter parameter)
{
	switch (parameter.mode)
	{
	case Immediate:
		return memory[address + parameter.offset];
	case Position:
		return memory[memory[address + parameter.offset]];
	}
}

bool Intcode::step()
{
	Opcode opcode(memory[ip]);

	switch (opcode.op)
	{
	case 1:
		memory[memory[ip + 3]] = decodeParameter(ip, opcode.getParameter(1)) + decodeParameter(ip, opcode.getParameter(2));
		ip += 4;
		break;
	case 2:
		memory[memory[ip + 3]] = decodeParameter(ip, opcode.getParameter(1)) * decodeParameter(ip, opcode.getParameter(2));
		ip += 4;
		break;
	case 3:
	{
		int value = input.front();
		input.pop();
		memory[memory[ip + 1]] = value;
		ip += 2;
		break;
	}
	case 4:
	{
		int value = decodeParameter(ip, opcode.getParameter(1));
		output.push(value);
		ip += 2;
		break;
	}
	case 5:
	{
		auto condition = decodeParameter(ip, opcode.getParameter(1));
		if (condition != 0)
			ip = decodeParameter(ip, opcode.getParameter(2));
		else
			ip += 3;
		break;
	}
	case 6:
	{
		auto condition = decodeParameter(ip, opcode.getParameter(1));
		if (condition == 0)
			ip = decodeParameter(ip, opcode.getParameter(2));
		else
			ip += 3;
		break;
	}
	case 7:
	{
		if (decodeParameter(ip, opcode.getParameter(1)) < decodeParameter(ip, opcode.getParameter(2)))
			memory[memory[ip + 3]] = 1;
		else
			memory[memory[ip + 3]] = 0;
		ip += 4;
		break;
	}
	case 8:
	{
		if (decodeParameter(ip, opcode.getParameter(1)) == decodeParameter(ip, opcode.getParameter(2)))
			memory[memory[ip + 3]] = 1;
		else
			memory[memory[ip + 3]] = 0;
		ip += 4;
		break;
	}
	case 99:
		return false;
	}

	return true;
}

void Intcode::run()
{
	ip = 0;
	while (step());
}