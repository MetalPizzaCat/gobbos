#include "keyboard.hpp"
#include <stdint.h>
#include <stddef.h>
#include <memory.hpp>
uint8_t os::keyboard::keycodeToAscii(Keycode kc, uint8_t isShifted)
{
	// if (isShifted)
	// {
	// 	return shifted_ascii_table[kc];
	// }
	// return ascii_table[kc];
}

os::keyboard::Keycode os::keyboard::scancodeToKeycode(uint8_t sc)
{
	sc &= 0b01111111; // remove released
	return (Keycode)sc;
}

bool os::keyboard::GlobalKeyboard::isKeyPressed(Keycode key) const
{
	if ((size_t)key > KEY_CODE_COUNT)
	{
		// TODO: Figure out if exception mechanism still works and use that
		return false;
	}
	return m_currentKeyStates[key];
}

bool os::keyboard::GlobalKeyboard::isKeyReleased(Keycode key) const
{
	if ((size_t)key > KEY_CODE_COUNT)
	{
		// TODO: Figure out if exception mechanism still works and use that
		return false;
	}
	return !m_currentKeyStates[key];
}

bool os::keyboard::GlobalKeyboard::isKeyJustPressed(Keycode key) const
{
	if ((size_t)key > KEY_CODE_COUNT)
	{
		// TODO: Figure out if exception mechanism still works and use that
		return false;
	}
	return m_currentKeyStates[key] && !m_previousKeyStates[key];
}

os::keyboard::GlobalKeyboard::GlobalKeyboard()
{
	memset(m_currentKeyStates, false, sizeof(m_currentKeyStates));
	memset(m_previousKeyStates, false, sizeof(m_previousKeyStates));
}
