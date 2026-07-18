#include <stdint.h>
#include "pit.h"
#include "utils.h"

void pit_set_timer(uint16_t hertz)
{
	uint32_t divisor = PIT_FREQUENCY / hertz;
	outb(PIT_COMMAND, PIT_SELECT_CHANNEL_0 | PIT_ACCESS_HI_LOW | PIT_MODE_SQUARE_WAVE);
	outb(PIT_CHANNEL_0, (uint8_t)(divisor & 0xFF));
	outb(PIT_CHANNEL_0, (uint8_t)((divisor >> 8) & 0xFF));
}