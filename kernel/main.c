
#include <os/os.h>
#include <memory.h>
#include "../games/invaders/invaders.h"

void update_game_logic(void);

// void set_fb(uint8_t value)
// {
// 	struct limine_framebuffer *framebuffer = fb_request.response->framebuffers[0];

// 	for (size_t y = 0; y < framebuffer->height; y++)
// 	{
// 		for (size_t x = 0; x < framebuffer->width * (framebuffer->bpp / 8); x++)
// 		{
// 			((uint8_t *)framebuffer->address)[x + y * framebuffer->pitch] = value;
// 		}
// 	}
// }

struct
{
	uint32_t x;
	uint32_t y;
	uint32_t w;
	uint32_t h;
} g_player;

void update_game_logic()
{
	// clear_framebuffer(0, 0, 0);
	// blit_color_rect(255, 0, 0, g_player.w, g_player.h, g_player.x, g_player.y);
	// if (g_key_states[KC_W])
	// {
	// 	g_player.y -= 10;
	// 	g_player.y %= (800 - g_player.h);
	// }
	// if (g_key_states[KC_S])
	// {
	// 	g_player.y += 10;
	// 	g_player.y %= (800 - g_player.h);
	// }
	// if (g_key_states[KC_D])
	// {
	// 	g_player.x += 10;
	// 	g_player.x %= (1280 - g_player.w);
	// }
	// if (g_key_states[KC_A])
	// {
	// 	g_player.x -= 10;
	// 	g_player.x %= (1280 - g_player.w);
	// }
	// else
	// {
	// 	clear_framebuffer(0, 0, 0);
	// }
}

/// This is the "main" main, which will just be running a menu and switch between games
void main()
{
	invaders_init_game();
	g_current_game_handlers.handle_input = invaders_handle_input;
	g_current_game_handlers.handle_update = invaders_update_game_logic;
}