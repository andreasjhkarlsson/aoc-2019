#include "intcode.h"

using std::queue;
using std::vector;
using std::optional;

enum Mode
{
	Position = 0,
	Immediate = 1,
	Relative = 2
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
	const int64_t op;
	Opcode(int64_t code) : op(code % 100)
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


Intcode::Intcode(const vector<int64_t>& memory) : memory(memory)
{
	registers.ip = 0;
	registers.rb = 0;
}

int64_t& Intcode::operator[](uint32_t address)
{
	if (address > 10000000)
		throw std::exception("out of memory");

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

int64_t& Intcode::decodeParameter(uint32_t address, Parameter parameter)
{
	switch (parameter.mode)
	{
	case Immediate:
		return (*this)[address + parameter.offset];
	case Position:
		return (*this)[(*this)[address + parameter.offset]];
	case Relative:
		return (*this)[registers.rb + (*this)[address + parameter.offset]];
	}
}

bool Intcode::step()
{
	Opcode opcode(memory[registers.ip]);

	switch (opcode.op)
	{
	case 1:
		decodeParameter(registers.ip, opcode.getParameter(3)) = decodeParameter(registers.ip, opcode.getParameter(1)) + decodeParameter(registers.ip, opcode.getParameter(2));
		registers.ip += 4;
		break;
	case 2:
		decodeParameter(registers.ip, opcode.getParameter(3)) = decodeParameter(registers.ip, opcode.getParameter(1)) * decodeParameter(registers.ip, opcode.getParameter(2));
		registers.ip += 4;
		break;
	case 3:
	{
		int64_t value = input.front();
		input.pop();
		decodeParameter(registers.ip,opcode.getParameter(1)) = value;
		registers.ip += 2;
		break;
	}
	case 4:
	{
		int64_t value = decodeParameter(registers.ip, opcode.getParameter(1));
		output.push(value);
		registers.ip += 2;
		break;
	}
	case 5:
	{
		auto condition = decodeParameter(registers.ip, opcode.getParameter(1));
		if (condition != 0)
			registers.ip = decodeParameter(registers.ip, opcode.getParameter(2));
		else
			registers.ip += 3;
		break;
	}
	case 6:
	{
		auto condition = decodeParameter(registers.ip, opcode.getParameter(1));
		if (condition == 0)
			registers.ip = decodeParameter(registers.ip, opcode.getParameter(2));
		else
			registers.ip += 3;
		break;
	}
	case 7:
	{
		if (decodeParameter(registers.ip, opcode.getParameter(1)) < decodeParameter(registers.ip, opcode.getParameter(2)))
			decodeParameter(registers.ip, opcode.getParameter(3)) = 1;
		else
			decodeParameter(registers.ip, opcode.getParameter(3)) = 0;
		registers.ip += 4;
		break;
	}
	case 8:
	{
		if (decodeParameter(registers.ip, opcode.getParameter(1)) == decodeParameter(registers.ip, opcode.getParameter(2)))
			decodeParameter(registers.ip, opcode.getParameter(3)) = 1;
		else
			decodeParameter(registers.ip, opcode.getParameter(3)) = 0;
		registers.ip += 4;
		break;
	}
	case 9:
	{
		registers.rb += decodeParameter(registers.ip, opcode.getParameter(1));
		registers.ip += 2;
		break;
	}
	case 99:
		return false;
	}

	return true;
}

void Intcode::run()
{
	registers.ip = 0;
	while (step());
}