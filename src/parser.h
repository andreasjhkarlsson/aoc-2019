#pragma once
#include "util.h"
#include <optional>
#include <string>
#include <initializer_list> 

namespace parser
{
	template <char C>
	std::optional<char> accept(const char** input)
	{
		if ((*input)[0] == C)
		{
			(*input)++;
			return C;
		}
		return std::nullopt;
	}


	template <typename T, std::optional<T>(*F)(const char**)>
	T expect(const char** input)
	{
		auto result = F(input);
		if (!result.has_value())
			throw new std::runtime_error("Invalid input");
		return result.value();
	}

	template <char ... CHARS>
	void expect(const char** input)
	{
		for (auto c : { CHARS... })
		{
			if ((*input)[0] == c)
				(*input)++;
			else
				throw new std::runtime_error("Invalid input");
		}
	}

	inline std::optional<char> digit(const char** input)
	{
		char c = (*input)[0];
		if (c >= '0' && c <= '9')
		{
			(*input)++;
			return c;
		}
		return std::nullopt;
	}

	inline std::optional<int> integer(const char** input)
	{
		auto first = digit(input);
		if (first)
		{
			int result = first.value() - '0';

			while (std::optional<char> d = digit(input))
			{
				result *= 10;
				result += d.value() - '0';
			}

			return result;
		}

		return std::nullopt;
	}

	inline std::optional<char> letter(const char** input)
	{
		auto c = (*input)[0];
		if (c >= 'A' && c <= 'Z')
		{
			(*input)++;
			return c;
		}
		return std::nullopt;
	}

	inline std::optional<std::string> text(const char** input)
	{
		auto first = letter(input);
		if (first.has_value())
		{
			std::string res(1, first.value());
			while (auto l = letter(input))
				res += l.value();

			return res;
		}

		return std::nullopt;
	}
}


