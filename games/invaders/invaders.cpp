#include "invaders.hpp"

#include <graphics/graphics.hpp>
#include <stdbool.h>
#include <os/os.hpp>

#define INVADER_ALIEN_CELL_WIDTH 16
#define INVADER_ALIEN_CELL_HEIGHT 24
#define INVADER_ALIEN_SCALE 2

typedef struct
{
	Rectangle collision;
	bool dead;
	uint8_t type;
} InvadersAlien;

typedef struct
{
	Rectangle collision;
} InvadersPlayer;

typedef struct
{
	InvadersAlien aliens[5 * 12];
	InvadersPlayer player;
	uint32_t ms_before_next_move;
} InvadersGameState;

InvadersGameState g_invaders_game_state;

void invaders_update_game_logic(void)
{

	static uint32_t game_frame_timer = 0;
	game_frame_timer += UPDATE_TIME;
	if (game_frame_timer > g_invaders_game_state.ms_before_next_move)
	{
		game_frame_timer = 0;
	}
	clear_screen((Color)MAKE_COLOR(0, 0, 0));

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			InvadersAlien const *alien = &g_invaders_game_state.aliens[i * 12 + j];
			if (alien->dead)
			{
				continue;
			}
			switch (alien->type)
			{
			case 0:
				draw_rect(alien->collision, (Color)MAKE_COLOR(255, 0, 0));
				break;
			case 1:
				draw_rect(alien->collision, (Color)MAKE_COLOR(0, 255, 0));
				break;
			case 2:
				draw_rect(alien->collision, (Color)MAKE_COLOR(0, 0, 255));
				break;
			}
		}
	}

	draw_rect(g_invaders_game_state.player.collision, (Color)MAKE_COLOR(255, 255, 255));
}

void invaders_handle_input(void)
{
	if (is_key_pressed(KC_A))
	{
		g_invaders_game_state.player.collision.x -= 5;
		if (g_invaders_game_state.player.collision.x < 0)
		{
			g_invaders_game_state.player.collision.x = 0;
		}
	}
	if (is_key_pressed(KC_D))
	{
		g_invaders_game_state.player.collision.x += 5;
		if (g_invaders_game_state.player.collision.x > 640 - g_invaders_game_state.player.collision.w)
		{
			g_invaders_game_state.player.collision.x = 640 - g_invaders_game_state.player.collision.w;
		}
	}
	if (is_key_just_pressed(KC_SPACE))
	{
	}
}

void invaders_init_game(void)
{
	uint32_t start_x = INVADER_ALIEN_CELL_WIDTH * 3 * INVADER_ALIEN_SCALE;
	uint32_t start_y = INVADER_ALIEN_CELL_HEIGHT * INVADER_ALIEN_SCALE;

	g_invaders_game_state.player.collision.x = 640 / 2;
	g_invaders_game_state.player.collision.y = INVADER_ALIEN_CELL_WIDTH * (3 + 6) * INVADER_ALIEN_SCALE;
	g_invaders_game_state.player.collision.w = 16;
	g_invaders_game_state.player.collision.h = 8;
	int alien_i = 0;
	// TODO: Adjust sized for each alien
	for (int i = 0; i < 12; i++)
	{
		g_invaders_game_state.aliens[alien_i++] = (InvadersAlien){
			(Rectangle){
				.x = i * INVADER_ALIEN_CELL_WIDTH * INVADER_ALIEN_SCALE + start_x,
				.y = start_y,
				.w = 8 * INVADER_ALIEN_SCALE,
				.h = 8 * INVADER_ALIEN_SCALE,
			},
			.dead = false,
			.type = 0};
	}

	for (int j = 0; j < 2; j++)
	{
		start_y += INVADER_ALIEN_CELL_HEIGHT * INVADER_ALIEN_SCALE;
		for (int i = 0; i < 12; i++)
		{
			g_invaders_game_state.aliens[alien_i++] = (InvadersAlien){
				(Rectangle){
					.x = i * INVADER_ALIEN_CELL_WIDTH * INVADER_ALIEN_SCALE + start_x,
					.y = start_y,
					.w = 8 * INVADER_ALIEN_SCALE,
					.h = 8 * INVADER_ALIEN_SCALE,
				},
				.dead = false,
				.type = 1};
		}
	}
	for (int j = 0; j < 2; j++)
	{
		start_y += INVADER_ALIEN_CELL_HEIGHT * INVADER_ALIEN_SCALE;
		for (int i = 0; i < 12; i++)
		{
			g_invaders_game_state.aliens[alien_i++] = (InvadersAlien){
				(Rectangle){
					.x = i * INVADER_ALIEN_CELL_WIDTH * INVADER_ALIEN_SCALE + start_x,
					.y = start_y,
					.w = 8 * INVADER_ALIEN_SCALE,
					.h = 8 * INVADER_ALIEN_SCALE,
				},
				.dead = false,
				.type = 2};
		}
	}
}