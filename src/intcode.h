#pragma once

#include <vector>
#include <sstream>
#include <queue>
#include <optional>
#include <thread>
#include <optional>
#include "util.h"

template <typename T>
class IO
{
private:
	std::queue<T> data;
	util::Semaphore semaphore;
	std::mutex mutex;
	bool ended = false;
public:
	void write(const T& value)
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

			if (ended)
				return std::nullopt;

			if (onRead)
			{
				auto data = onRead();
				if (data.has_value())
				{

					return data.value();
				}
				else
				{
					ended = true;
					return std::nullopt;
				}
			}
			else
			{
				auto value = data.front();
				data.pop();
				return value;
			}
		}
	}

	bool empty()
	{
		return data.size() == 0;
	}

	std::function<std::optional<T>()> onRead;
};


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

class Intcode
{
private:
	Memory<int64_t> memory;
	uint32_t ip;
	int32_t rb;
	bool halted;
	IO<int64_t> input;
	IO<int64_t> output;
	std::unique_ptr<std::thread> runner;

	void loadParameters(uint32_t address, int count, int64_t** params);

public:
	Intcode(const std::vector<int64_t>& program);
	Intcode(const Intcode& other);
	~Intcode();

	void run(bool wait = true);
	void wait();
	bool isHalted();

	IO<int64_t>& getInput();
	IO<int64_t>& getOutput();
	Memory<int64_t>& getMemory();
};