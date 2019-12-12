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
			params[i] = &memory[(uint32_t)memory[address + i + 1]];
			break;
		case 1:
			params[i] = &memory[address + i + 1];
			break;
		case 2:
			params[i] = &memory[rb + (uint32_t)memory[address + i + 1]];
			break;
		
		}
		modes /= 10;
	}

	// If memory was reallocated during parameter lookup, we
	// need to recalculate parameters since pointers are now invalid
	if (memory.size() != memorySize)
		loadParameters(address, count, params);
}

#pragma warning(push)
#pragma warning(disable: 4102)
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
		case 1: add:
		{
			loadParameters(3);
			*params[2] = *params[0] + *params[1];
			ip += 4;
			break;
		}

		case 2: mul:
		{
			loadParameters(3);
			*params[2] = *params[0] * *params[1];
			ip += 4;
			break;
		}
		case 3: read:
		{
			loadParameters(1);
			auto read = input.read();
			if (!read)
				goto panic; // We can't handle EOF on input
			*params[0] = read.value();
			ip += 2;
			break;
		}
		case 4: write:
		{
			loadParameters(1);
			output.write(*params[0]);
			ip += 2;
			break;
		}
		case 5: jump_nez:
		{
			loadParameters(2);
			if (*params[0] != 0)
				ip = (uint32_t)*params[1];
			else
				ip += 3;
			break;
		}
		case 6: jump_ez:
		{
			loadParameters(2);
			if (*params[0] == 0)
				ip = (uint32_t)*params[1];
			else
				ip += 3;
			break;
		}
		case 7: lesser:
		{
			loadParameters(3);
			if (*params[0] < *params[1])
				*params[2] = 1;
			else
				*params[2] = 0;
			ip += 4;
			break;
		}
		case 8: equals:
		{
			loadParameters(3);
			if (*params[0] == *params[1])
				*params[2] = 1;
			else
				*params[2] = 0;
			ip += 4;
			break;
		}
		case 9: set_rb:
		{
			loadParameters(1);
			rb += (int32_t)*params[0];
			ip += 2;
			break;
		}
		case 99: halt:
			output.setEOF();
			return;
		default: panic:
			std::cerr << "Panic! Intcode computer encountered an unrecoverable state." << std::endl;
			goto halt;
		}

		goto step;
	});

	if (wait)
		this->wait();
}
#pragma warning(pop)

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

IO<int64_t>& Intcode::getInput()
{
	return input;
}
IO<int64_t>& Intcode::getOutput()
{
	return output;
}

Memory<int64_t>& Intcode::getMemory()
{
	return memory;
}