#include "invaders.hpp"

#include <graphics/graphics.hpp>
#include <stdbool.h>
#include <os/os.hpp>
#include <keyboard/keyboard.hpp>
#include <string.hpp>
#include <os/io.hpp>
#include <math.hpp>

#define INVADER_ALIEN_CELL_WIDTH 16
#define INVADER_ALIEN_CELL_HEIGHT 24
#define INVADER_ALIEN_SCALE 2

void Games::Invaders::Object::draw()
{
	Graphics::getInstance().fillRect(m_collision, m_color);
}

void Games::Invaders::Object::setPosition(Vec2i position)
{
	m_collision.x = position.x;
	m_collision.y = position.y;
}

std::geometry::Vec2i Games::Invaders::Object::getPosition() const
{
	return std::geometry::Vec2i(m_collision.x, m_collision.y);
}

std::geometry::Vec2i Games::Invaders::Object::getSize() const
{
	return std::geometry::Vec2i(m_collision.w, m_collision.h);
}
void Games::Invaders::GameState::init()
{

	uint32_t start_x = constants::alienCellWidth * 3 * constants::alienGameScale;
	uint32_t start_y = constants::alienCellHeight * constants::alienGameScale;

	m_player = Object(
		Rect(Graphics::getInstance().getScreenSize().x / 2, Graphics::getInstance().getScreenSize().y - 64, 16, 8),
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
	m_alienTimeSinceLastMove += (float)UPDATE_TIME;
	if (m_alienTimeSinceLastMove > m_alienMoveTime || true)
	{
		m_alienTimeSinceLastMove = 0;

		for (int i = 0; i < 5 * 12; i++)
		{
			m_aliens[i].setPosition(Vec2i(m_aliens[i].getPosition().x + (m_movingLeft ? -1 : 1), m_aliens[i].getPosition().y + 8 * m_moveDownOnNextTurn));
		}
		if (m_moveDownOnNextTurn)
		{
			m_moveDownOnNextTurn = false;
		}
	}
	Graphics::getInstance().clearScreen(Color(0, 0, 0));
	m_player.draw();

	for (int i = 0; i < 5 * 12; i++)
	{
		Alien &alien = m_aliens[i];

		if (alien.isDead())
		{
			continue;
		}
		if (m_aliens[i].getPosition().x <= 0 || (m_aliens[i].getPosition().x + m_aliens[i].getSize().x) > Graphics::getInstance().getScreenSize().x)
		{
			m_moveDownOnNextTurn = true;
			m_movingLeft = !m_movingLeft;
		}
		alien.draw();
	}
}

void Games::Invaders::GameState::handleInput()
{
	using namespace os::keyboard;
	Vec2i newPos = m_player.getPosition();
	if (GlobalKeyboard::getInstance().isKeyPressed(KC_A))
	{
		newPos.x = max(newPos.x - 5, 0ul);
	}
	if (GlobalKeyboard::getInstance().isKeyPressed(KC_D))
	{
		newPos.x = min(newPos.x + 5, Graphics::getInstance().getScreenSize().x - m_player.getSize().x);
	}

	m_player.setPosition(newPos);
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
