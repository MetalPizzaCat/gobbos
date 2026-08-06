#include "graphics.hpp"
#include <os/limine.h>
#include <memory.hpp>
#include <math.hpp>
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
		const uint64_t xStart = max(0ul, rect.x);
		const uint64_t xEnd = min(m_framebuffer->width, rect.x + rect.w);
		if (xStart >= xEnd || h + rect.y > m_framebuffer->height)
		{
			continue;
		}
		mempattern(c, (uint8_t *)m_framebuffer->address + ((rect.y + h) * m_framebuffer->pitch) + (xStart * m_framebuffer->bpp / 8), xEnd - xStart);
	}
}

void os::graphics::Graphics::drawTexture(Texture const *texture, Vec2i pos, Rect *sourceRect)
{
	if (texture == nullptr)
	{
		return;
	}
	// memcpy((uint8_t *)m_framebuffer->address + pos.x + (pos.y + 0) * m_framebuffer->pitch, texture->getData(), 4 * texture->getWidth() + 0 * texture->getWidth() * 4);

	for (uint64_t y = 0; y < texture->getHeight(); y++)
	{
		if (pos.y + y > m_framebuffer->height)
		{
			continue;
		}
		// to avoid going out of bounds of memory we add a check
		const uint64_t xStart = max(0ul, pos.x);
		const uint64_t xEnd = min(m_framebuffer->width, pos.x + texture->getWidth());
		if (xStart >= xEnd)
		{
			continue;
		}
		uint64_t yAdjusted = texture->isUpsideDown() ? (texture->getHeight() - y - 1) : y;
		// mempattern(*reinterpret_cast<uint32_t *>(texture->getData()) & 0x00FF0000, (uint8_t *)m_framebuffer->address + pos.x * 4 + (pos.y + y) * m_framebuffer->pitch, texture->getWidth());
		memcpy((uint8_t *)m_framebuffer->address + xStart * 4 + (pos.y + y) * m_framebuffer->pitch, texture->getData() + yAdjusted * texture->getWidth() * 4, 4 * (xEnd - xStart));
		//  memcpy((uint8_t *)m_framebuffer->address + pos.x + (pos.y + y) * m_framebuffer->pitch, texture->getData() + y * 4 * texture->getWidth(), 4 * texture->getWidth() + y * texture->getWidth() * 4);
	}
}

os::graphics::Color::Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b)
{
}
