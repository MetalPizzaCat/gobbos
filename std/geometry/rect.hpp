#pragma once
#include <stdint.h>
namespace std::geometry
{
	template <typename T>
	struct Rectangle
	{
		T x;
		T y;
		T w;
		T h;

		T getX() const { return x; }
		T getY() const { return y; }
		T getWidth() const { return w; }
		T getHeight() const { return h; }

		explicit Rectangle() = default;
		explicit Rectangle(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}
	};

	using Rect = Rectangle<uint64_t>;
}