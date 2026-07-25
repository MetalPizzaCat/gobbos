#include "serial.hpp"
#include "utils.h"
os::serial::Serial::Serial() = default;

bool os::serial::Serial::init()
{
	outb(SERIAL_PORT + 1, 0);							   // disable all interrupts
	outb(SERIAL_PORT + 3, 1 << 7);						   // enable DLAB bit to set the baud rate divisor
	outb(SERIAL_PORT + 0, SERIAL_FREQUENCY / 9600);		   // low byte
	outb(SERIAL_PORT + 1, (SERIAL_FREQUENCY / 9600) >> 8); // high byte
	outb(SERIAL_PORT + 3, 0b11);						   // set character length to 8 bits (and stop bits to 1, and parity to none, but those values are set with 0s)
	outb(SERIAL_PORT + 2, 0b110);						   // clear FIFO buffers

	outb(SERIAL_PORT + 4, 0b10000); // set into loop back mode to test that it is working
	outb(SERIAL_PORT + 0, 0xAE);	// transmit data
	if (inb(SERIAL_PORT + 0) != 0xAE)
	{
		return false;
	}
	outb(SERIAL_PORT + 4, 0b01111); // set to enable interrupts and data terminal ready pin and request to send pin
	return true;
}

uint8_t os::serial::Serial::readByte()
{
	while (!hasReceived())
		;
	return inb(SERIAL_PORT);
}

size_t os::serial::Serial::readUntil(uint8_t *buffer, size_t length, char delimiter)
{
	for (size_t i = 0; i < length; i++)
	{
		uint8_t byte = readByte();
		buffer[i] = byte;
		if (byte == (uint8_t)delimiter)
		{
			return i + 1;
		}
	}
	return length;
}

void os::serial::Serial::writeByte(uint8_t byte)
{
	while (!isBufferEmpty())
		;
	outb(SERIAL_PORT, byte);
}

void os::serial::Serial::writeBuffer(uint8_t *buffer, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		writeByte(buffer[i]);
	}
}

void os::serial::Serial::writeCString(const char *str)
{
	while (*str != '\0')
	{
		writeByte(*str++);
	}
}

bool os::serial::Serial::isBufferEmpty() const
{
	return inb(SERIAL_PORT + 5) & 0b100000;
}

bool os::serial::Serial::hasReceived() const
{
	return inb(SERIAL_PORT + 5) & 0b1;
}

void os::serial::Serial::clear()
{
	writeCString("\033[H\033[2J");
}