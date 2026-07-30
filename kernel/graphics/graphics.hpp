#pragma once
#include <stddef.h>
#include <stdint.h>
#include <os/limine.h>
#include <geometry/rect.hpp>
#include <geometry/point.hpp>
#include <graphics/texture.hpp>

//__attribute__((section(".limine_requests"))) extern struct limine_framebuffer_request fb_request;

namespace os::graphics
{
	using namespace std::geometry;

	struct Color
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;

		Color(uint8_t r, uint8_t g, uint8_t b);

		/// @brief Create color from single number with 8 bits per color in RGBA format
		/// @param color
		Color(uint32_t color);

		Color() = default;
	};

	class Graphics
	{
	public:
		explicit Graphics();
		static Graphics &getInstance()
		{
			static Graphics g;
			return g;
		}

		/// @brief Clear entire screen to a given color value
		/// @param color Color to clear screen to(alpha is ignored)
		void clearScreen(Color color);

		/// @brief Draw solid color rectangle(alpha is ignored) with given dimensions
		/// @param rect Rectangle to draw
		/// @param color Color of the rectangle
		void fillRect(Rect rect, Color color);

		/// @brief Draw the texture on screen. No alpha channel calculation is performed, image will override all pixels
		/// @param pos Location on the screen to draw texture at
		/// @param sourceRect Section of the texture to draw or full texture if it's null
		void drawTexture(Texture const *texture, Vec2i pos, Rect *sourceRect = nullptr);

		Vec2i getScreenSize() const
		{
			return Vec2i(m_framebuffer->width, m_framebuffer->height);
		}

	private:
		LimineFramebuffer *m_framebuffer;
		LimineFramebufferResponse const *m_limineFramebufferResponse;
	};

}
