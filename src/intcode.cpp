#include "intcode.h"

using std::queue;
using std::vector;
using std::optional;
using std::thread;


Intcode::Intcode(const vector<int64_t>& program):
	memory(program),
	ip(0),
	rb(0),
	halted(false)
{
}

Intcode::Intcode(const Intcode& other):
	memory(other.memory),
	ip(other.ip),
	rb(other.rb),
	halted(other.halted)
{
	if (other.runner)
		throw std::exception("Cannot copy a running Intcode computer");
}

Intcode::~Intcode()
{
	wait();
}

void Intcode::loadParameters(uint32_t address, int count, int64_t** params)
{
	size_t memorySize = memory.size();

	int64_t modes = memory[address] / 100;

	for (int i = 0; i < count; i++)
	{
		switch (modes % 10)
		{
		case 0:
			params[i] = &memory[memory[address + i + 1]];
			break;
		case 1:
			params[i] = &memory[address + i + 1];
			break;
		case 2:
			params[i] = &memory[rb + memory[address + i + 1]];
			break;
		
		}
		modes /= 10;
	}

	// If memory was reallocated during parameter lookup, we
	// need to recalculate parameters since pointers are now invalid
	if (memory.size() != memorySize)
		loadParameters(address, count, params);
}

void Intcode::run(bool wait)
{
	runner = std::make_unique<thread>([this]() {

		step:

		int64_t* params[4];

		auto loadParameters = [this, &params](int count) {
			this->loadParameters(ip, count, params);
		};

		switch (memory[ip]%100)
		{
		case 1:
		{
			loadParameters(3);
			*params[2] = *params[0] + *params[1];
			ip += 4;
			break;
		}

		case 2:
		{
			loadParameters(3);
			*params[2] = *params[0] * *params[1];
			ip += 4;
			break;
		}
		case 3:
		{
			loadParameters(1);
			int64_t value = input.read();
			*params[0] = value;
			ip += 2;
			break;
		}
		case 4:
		{
			loadParameters(1);
			output.write(*params[0]);
			ip += 2;
			break;
		}
		case 5:
		{
			loadParameters(2);
			if (*params[0] != 0)
				ip = *params[1];
			else
				ip += 3;
			break;
		}
		case 6:
		{
			loadParameters(2);
			if (*params[0] == 0)
				ip = *params[1];
			else
				ip += 3;
			break;
		}
		case 7:
		{
			loadParameters(3);
			if (*params[0] < *params[1])
				*params[2] = 1;
			else
				*params[2] = 0;
			ip += 4;
			break;
		}
		case 8:
		{
			loadParameters(3);
			if (*params[0] == *params[1])
				*params[2] = 1;
			else
				*params[2] = 0;
			ip += 4;
			break;
		}
		case 9:
		{
			loadParameters(1);
			rb += *params[0];
			ip += 2;
			break;
		}
		case 99:
			output.setEOF();
			return;
		}

		goto step;
	});

	if (wait)
		this->wait();
}
void Intcode::wait()
{
	if (runner)
		runner->join();

	runner.reset();
}

bool Intcode::isHalted()
{
	return halted;
}

BlockingQueue<int64_t>& Intcode::getInput()
{
	return input;
}
BlockingQueue<int64_t>& Intcode::getOutput()
{
	return output;
}

Memory<int64_t>& Intcode::getMemory()
{
	return memory;
}