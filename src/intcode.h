#pragma once

#include <vector>
#include <sstream>
#include <queue>
#include <optional>
#include <thread>
#include "util.h"

template <typename T>
class BlockingQueue
{
private:
	std::queue<T> data;
	util::Semaphore semaphore;
	std::mutex mutex;
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
		semaphore.notify();
	}

	T read()
	{
		bool _; return read(_);
	}

	T read(bool& eof)
	{
		eof = false;
		semaphore.wait();

		{
			std::lock_guard<std::mutex> guard(mutex);

			if (data.size() == 0)
			{
				eof = true;
				return T();
			}

			auto value = data.front();
			data.pop();
			return value;
		}
	}

	bool empty()
	{
		return data.size() == 0;
	}
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
	BlockingQueue<int64_t> input;
	BlockingQueue<int64_t> output;
	std::unique_ptr<std::thread> runner;

	void loadParameters(uint32_t address, int count, int64_t** params);

public:
	Intcode(const std::vector<int64_t>& program);
	Intcode(const Intcode& other);
	~Intcode();

	void run(bool wait = true);
	void wait();
	bool isHalted();

	BlockingQueue<int64_t>& getInput();
	BlockingQueue<int64_t>& getOutput();
	Memory<int64_t>& getMemory();
};