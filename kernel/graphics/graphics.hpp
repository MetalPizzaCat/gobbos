#pragma once
#include <stddef.h>
#include <stdint.h>

//__attribute__((section(".limine_requests"))) extern struct limine_framebuffer_request fb_request;

typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} Color;

typedef struct
{
	uint32_t x;
	uint32_t y;
	uint32_t w;
	uint32_t h;
} Rectangle;

#define MAKE_COLOR(red, green, blue) {.r = red, .g = green, .b = blue}

/// @brief Clear entire screen to a given color value
/// @param color Color to clear screen to(alpha is ignored)
void clear_screen(Color color);

/// @brief Draw solid color rectangle(alpha is ignored) with given dimensions
/// @param rect Rectangle to draw
/// @param color Color of the rectangle
void draw_rect(Rectangle rect, Color color);