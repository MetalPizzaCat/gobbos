#pragma once
#include <stdint.h>
namespace std::geometry
{
	template <typename T>
	struct Vec2
	{
		T x;
		T y;

		Vec2(T x, T y) : x(x), y(y) {}
	};

	using Vec2i = Vec2<uint64_t>;
}