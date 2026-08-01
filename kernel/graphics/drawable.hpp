#pragma once
#include <geometry/point.hpp>
namespace os::graphics
{
	class Drawable
	{
	public:
		virtual void draw() = 0;

		virtual ~Drawable() = default;

	protected:
		std::geometry::Vec2i m_position = std::geometry::Vec2i(0, 0);
	};
} // namespace os::graphics
