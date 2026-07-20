#include "graphics.hpp"
#include <os/limine.h>
#include <memory.hpp>

__attribute__((section(".limine_requests"))) struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST_ID,
};

void clear_screen(Color color)
{
	// pixel access is `offset = (y * pitch) + (x * bpp / 8)`

	// assuming that buffer is 32bit but ideally we shouldn't

	struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
	mempattern4(
		(color.r << framebuffer->red_mask_shift) |
			(color.g << framebuffer->green_mask_shift) |
			(color.b << framebuffer->blue_mask_shift),
		framebuffer->address, framebuffer->width * framebuffer->height);
}

void draw_rect(Rectangle rect, Color color)
{
	struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
	uint32_t c = (color.r << framebuffer->red_mask_shift) | (color.g << framebuffer->green_mask_shift) | (color.b << framebuffer->blue_mask_shift);
	for (uint32_t h = 0; h < rect.h; h++)
	{
		mempattern4(c, (uint8_t *)framebuffer->address + ((rect.y + h) * framebuffer->pitch) + (rect.x * framebuffer->bpp / 8), rect.w);
	}
}
