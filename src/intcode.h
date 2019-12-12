#pragma once

#include <vector>
#include <sstream>
#include <queue>
#include <optional>
#include <thread>
#include <optional>
#include "util.h"

template <typename T>
class IODevice
{
public:
	virtual void write(const T& value) = 0;
	virtual void setEOF() = 0;
	virtual std::optional<T> read() = 0;
	void readAll(std::vector<T> data)
	{
		std::optional<T> value;
		fetch: value = read();
		if (value.has_value())
		{
			data.push_back(value);
			goto fetch;
		}

	}
};

template <typename T>
class InputDevice : public IODevice<T>
{
public:
	virtual void write(const T& value) override {}
	virtual void setEOF() override {}
};

template <typename T>
class OutputDevice : public IODevice<T>
{
public:
	virtual std::optional<T> read() override
	{
		return std::nullopt;
	}
};

template <typename T>
class BufferedIODevice : public IODevice<T>
{
private:
	std::queue<T> data;
	util::Semaphore semaphore;
	std::mutex mutex;
	bool ended = false;
public:

	BufferedIODevice(const std::vector<T>& data = std::vector<T>())
	{
		for (const auto& e : data)
			this->data.push(e);
	}

	virtual void write(const T& value) override
	{
		{
			std::lock_guard<std::mutex> guard(mutex);
			data.push(value);
		}
		semaphore.notify();
	}

	void setEOF()
	{
		ended = true;
		semaphore.notify();
	}

	std::optional<T> read()
	{
		semaphore.wait();
		{
			std::lock_guard<std::mutex> guard(mutex);

			if (ended && data.size() == 0)
				return std::nullopt;

			auto value = data.front();
			data.pop();
			return value;
		}
	}

	void readAll(std::vector<T>& data)
	{
		auto r = this->read();
		if (r)
		{
			data.push_back(r.value());
			readAll(data);
		}
	}

	bool empty()
	{
		return data.size() == 0;
	}
};

template<typename IN, typename OUT>
struct IO
{
	IN input;
	OUT output;
};

typedef IO<BufferedIODevice<int64_t>, BufferedIODevice<int64_t>> BufferedIO;

template <typename T>
class Memory
{
private:
	std::vector<T> data;
	uint32_t limit;
public:
	Memory(const std::vector<T>& data, uint32_t limit = 1024*1024): data(data), limit(limit)
	{ }

	T& operator[](uint32_t address)
	{
		if (address > limit)
			throw std::exception("out of memory");

		if (address >= data.size())
			data.resize(address + 1);

		return data[address];
	}

	size_t size()
	{
		return data.size();
	}
};

template <typename T = BufferedIO>
class Intcode
{
private:
	Memory<int64_t> memory;
	uint32_t ip;
	int32_t rb;
	bool halted;
	T io;
	std::unique_ptr<std::thread> runner;

	void loadParameters(uint32_t address, int count, int64_t** params)
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

public:
	Intcode(const std::vector<int64_t>& program):
		memory(program),
		ip(0),
		rb(0),
		halted(false)
	{

	}

	Intcode(const std::vector<int64_t>& program, T&& io) :
		memory(program),
		ip(0),
		rb(0),
		halted(false),
		io(io)
	{

	}

	~Intcode()
	{
		wait();
	}

	#pragma warning(push)
	#pragma warning(disable: 4102)
	void run(bool wait = true)
	{
		runner = std::make_unique<std::thread>([this]() {

		step:

			int64_t* params[4];

			auto loadParameters = [this, &params](int count) {
				this->loadParameters(ip, count, params);
			};

			switch (memory[ip] % 100)
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
				auto read = io.input.read();
				if (!read)
					goto panic; // We can't handle EOF on input
				*params[0] = read.value();
				ip += 2;
				break;
			}
			case 4: write:
			{
				loadParameters(1);
				io.output.write(*params[0]);
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
			io.output.setEOF();
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

	void wait()
	{
		if (runner)
			runner->join();

		runner.reset();
	}

	bool isHalted()
	{
		return halted;
	}

	Memory<int64_t>& getMemory()
	{
		return memory;
	}

	decltype(io.input)& getInputDevice()
	{
		return io.input;
	}

	decltype(io.output)& getOutputDevice()
	{
		return io.output;
	}
};

template<typename T, typename U>
Intcode<T>& operator<<(Intcode<T>& computer, U value)
{
	computer.getInputDevice().write(value);
	return computer;
}

template<typename T, typename U>
Intcode<T>& operator>>(Intcode<T>& computer, U& value)
{
	value = computer.getOutputDevice().read().value();
	return computer;
}

template<typename T, typename U>
Intcode<T>& operator>>(Intcode<T>& computer, std::optional<U>& value)
{
	value = computer.getOutputDevice().read();
	return computer;
}

