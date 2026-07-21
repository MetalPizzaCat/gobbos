#pragma once
#include <geometry/point.hpp>
#include <geometry/rect.hpp>
#include <graphics/graphics.hpp>

namespace Games::Invaders
{
	using namespace std::geometry;
	using namespace os::graphics;

	namespace constants
	{
		const uint32_t alienCellWidth = 16;
		const uint32_t alienCellHeight = 24;
		const uint32_t alienGameScale = 2;
	}
	class Object
	{
	public:
		explicit Object() = default;
		explicit Object(Rect collision, Color color) : m_collision(collision), m_color(color) {}

		explicit Object(Rect collision, uint8_t type) : m_collision(collision), m_color(255, 0, 0) {}
		void draw();

		inline bool isDead() const { return m_dead; }

	private:
		Rect m_collision;
		bool m_drawDebugCollision;
		Color m_color;
		bool m_dead = false;
		uint8_t m_type;
	};

	class GameState
	{
	public:
		explicit GameState() = default;
		static GameState &getInstance()
		{
			static GameState state = GameState();
			return state;
		}

		void init();

		void update();

		void handleInput();

	private:
		Object m_aliens[5 * 12];
		Object m_player;
		uint32_t m_alienMoveTime = 1;
		uint32_t m_alienTimeSinceLastMove = 0;
	};

	void handleInput();

	void updateGameLogic();

	void init();

} // namespace Games::Invaders
