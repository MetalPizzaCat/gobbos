#include "graphics.hpp"
#include <os/limine.h>
#include <memory.hpp>

__attribute__((section(".limine_requests"))) struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST_ID,
};


os::graphics::Graphics::Graphics() : m_limineFramebufferResponse(framebuffer_request.response), m_framebuffer(framebuffer_request.response->framebuffers[0])
{
}

void os::graphics::Graphics::clearScreen(Color color)
{
	mempattern<uint32_t>(
		(color.r << m_framebuffer->red_mask_shift) |
			(color.g << m_framebuffer->green_mask_shift) |
			(color.b << m_framebuffer->blue_mask_shift),
		m_framebuffer->address, m_framebuffer->width * m_framebuffer->height);
}

void os::graphics::Graphics::fillRect(Rect rect, Color color)
{
	uint32_t c = (color.r << m_framebuffer->red_mask_shift) | (color.g << m_framebuffer->green_mask_shift) | (color.b << m_framebuffer->blue_mask_shift);
	for (uint32_t h = 0; h < rect.h; h++)
	{
		mempattern(c, (uint8_t *)m_framebuffer->address + ((rect.y + h) * m_framebuffer->pitch) + (rect.x * m_framebuffer->bpp / 8), rect.w);
	}
}

os::graphics::Color::Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b)
{
}


