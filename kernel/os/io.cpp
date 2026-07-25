#include "io.hpp"
#include "serial.hpp"

void os::io::log(const char *msg)
{
	os::serial::Serial::getInstance().writeCString(msg);
}