#pragma once

#include <string>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <functional>
#include <set>
#include <vector>
#include <type_traits>
#include <mutex>
#include <memory>
#include <condition_variable>

namespace util
{
	// https://stackoverflow.com/a/17976541/242348
	inline std::string trim(const std::string &s)
	{
		auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) {return std::isspace(c); });
		return std::string(wsfront, std::find_if_not(s.rbegin(), std::string::const_reverse_iterator(wsfront), [](int c) {return std::isspace(c); }).base());
	}

	template <typename T>
	T convert(const std::string& str)
	{
		T result;
		std::istringstream(str) >> result;
		return result;
	}

	template <typename T, typename U>
	std::vector<U> mapVector(const std::vector<T>& vector, std::function<U(T)> fn = util::convert<U>)
	{
		std::vector<U> mapped(vector.size());
		std::transform(vector.begin(), vector.end(), mapped.begin(), fn);
		return mapped;
	}

	// https://stackoverflow.com/a/46931770/242348
	inline std::vector<std::string> split(const std::string &s, char delim)
	{
		std::vector<std::string> result;
		std::stringstream ss(s);
		std::string item;

		while (std::getline(ss, item, delim)) {
			result.push_back(item);
		}

		return result;
	}

	template <typename T>
	bool contains(const T& s, typename T::value_type const & e)
	{
		return std::find(s.begin(), s.end(), e) != s.end();
	}

	struct Coord
	{
		int x;
		int y;
		Coord(int x, int y): x(x), y(y)
		{ }
	};

	inline bool operator<(const Coord& l, const Coord& r)
	{
		return (l.x < r.x || (l.x == r.x && l.y < r.y));
	}

	inline bool operator==(const Coord& l, const Coord& r)
	{
		return l.x == r.x && l.y == r.y;
	}

	// https://stackoverflow.com/a/27697079/242348
	inline std::vector<std::vector<int>> permuteUnique(std::vector<int> num) {
		std::sort(num.begin(), num.end());
		std::vector<std::vector<int> > res;
		if (num.empty()) {
			return res;
		}
		do {
			res.push_back(num);
		} while (std::next_permutation(num.begin(), num.end()));
		return res;
	}

	// https://stackoverflow.com/a/4793662/242348
	class Semaphore
	{
	private:
		std::mutex mutex_;
		std::condition_variable condition_;
		unsigned long count_ = 0; // Initialized as locked.

	public:
		void notify() {
			std::lock_guard<decltype(mutex_)> lock(mutex_);
			++count_;
			condition_.notify_one();
		}

		void wait() {
			std::unique_lock<decltype(mutex_)> lock(mutex_);
			while (!count_) // Handle spurious wake-ups.
				condition_.wait(lock);
			--count_;
		}

		bool try_wait() {
			std::lock_guard<decltype(mutex_)> lock(mutex_);
			if (count_) {
				--count_;
				return true;
			}
			return false;
		}
	};
}

namespace std
{
	template <>
	struct hash<util::Coord>
	{
		size_t operator()(const util::Coord& k) const
		{
			auto h1 = std::hash<int>()(k.x);
			auto h2 = std::hash<int>()(k.y);
			return h1 ^ h2;
		}
	};
}