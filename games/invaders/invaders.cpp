#include "invaders.hpp"

#include <graphics/graphics.hpp>
#include <stdbool.h>
#include <os/os.hpp>
#include <keyboard/keyboard.hpp>

#define INVADER_ALIEN_CELL_WIDTH 16
#define INVADER_ALIEN_CELL_HEIGHT 24
#define INVADER_ALIEN_SCALE 2

void Games::Invaders::Object::draw()
{
	Graphics::getInstance().fillRect(m_collision, m_color);
}
void Games::Invaders::GameState::init()
{
	uint32_t start_x = constants::alienCellWidth * 3 * constants::alienGameScale;
	uint32_t start_y = constants::alienCellHeight * constants::alienGameScale;

	m_player = Object(
		Rect(Graphics::getInstance().getScreenSize().x / 2, constants::alienCellWidth * 9 * constants::alienGameScale, 16, 8),
		Color(255, 255, 255));

	int alien_i = 0;
	// TODO: Adjust sized for each alien
	for (int i = 0; i < 12; i++)
	{
		m_aliens[alien_i++] = Alien(Rect(
										i * constants::alienCellWidth * constants::alienGameScale + start_x,
										start_y,
										8 * constants::alienGameScale,
										8 * constants::alienGameScale),
									0);
	}

	for (int j = 0; j < 2; j++)
	{
		start_y += INVADER_ALIEN_CELL_HEIGHT * INVADER_ALIEN_SCALE;
		for (int i = 0; i < 12; i++)
		{

			m_aliens[alien_i++] = Alien(Rect(
											i * constants::alienCellWidth * constants::alienGameScale + start_x,
											start_y,
											8 * constants::alienGameScale,
											8 * constants::alienGameScale),
										1);
		}
	}
	for (int j = 0; j < 2; j++)
	{
		start_y += INVADER_ALIEN_CELL_HEIGHT * INVADER_ALIEN_SCALE;
		for (int i = 0; i < 12; i++)
		{
			m_aliens[alien_i++] = Alien(Rect(
											i * constants::alienCellWidth * constants::alienGameScale + start_x,
											start_y,
											8 * constants::alienGameScale,
											8 * constants::alienGameScale),
										2);
		}
	}
}

void Games::Invaders::GameState::update()
{
	m_alienTimeSinceLastMove += UPDATE_TIME;
	if (m_alienTimeSinceLastMove > m_alienMoveTime)
	{
		m_alienTimeSinceLastMove = 0;
	}
	Graphics::getInstance().clearScreen(Color(0, 0, 0));
	m_player.draw();
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			Alien &alien = m_aliens[i * 12 + j];
			if (alien.isDead())
			{
				continue;
			}
			alien.draw();
		}
	}
}
void Games::Invaders::GameState::handleInput()
{
	using namespace os::keyboard;
	if (GlobalKeyboard::getInstance().isKeyPressed(KC_A))
	{
		// g_invaders_game_state.player.collision.x -= 5;
		// if (g_invaders_game_state.player.collision.x < 0)
		// {
		// 	g_invaders_game_state.player.collision.x = 0;
		// }
	}
	if (GlobalKeyboard::getInstance().isKeyPressed(KC_D))
	{
		// g_invaders_game_state.player.collision.x += 5;
		// if (g_invaders_game_state.player.collision.x > 640 - g_invaders_game_state.player.collision.w)
		// {
		// 	g_invaders_game_state.player.collision.x = 640 - g_invaders_game_state.player.collision.w;
		// }
	}
	if (GlobalKeyboard::getInstance().isKeyJustPressed(KC_SPACE))
	{
	}
}

void Games::Invaders::init()
{
	GameState::getInstance().init();
}

void Games::Invaders::handleInput() { GameState::getInstance().handleInput(); }

void Games::Invaders::updateGameLogic() { GameState::getInstance().update(); }
