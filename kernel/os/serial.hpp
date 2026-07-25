#pragma once
#include <stdint.h>
#include <stddef.h>

#define SERIAL_PORT 0x3f8
#define SERIAL_FREQUENCY 115200

namespace os::serial
{

	/// @brief Singleton class for interacting with the serial port of the hardware
	class Serial
	{
	public:
		explicit Serial();
		static Serial &getInstance()
		{
			static Serial s = Serial();
			return s;
		}

		bool init();

		uint8_t readByte();

		size_t readUntil(uint8_t *buffer, size_t length, char delimiter = '\n');

		void writeByte(uint8_t byte);

		void writeBuffer(uint8_t *buffer, size_t length);

		void writeCString(const char *str);

		/// @brief Clear the screen and reset cursor to the initial position
		void clear();

	protected:
		bool isBufferEmpty() const;

		bool hasReceived() const;

	private:
		bool m_enabled;
	};
} // namespace os::serial
