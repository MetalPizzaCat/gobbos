#pragma once
#include <stddef.h>
#include <stdint.h>
// https://wiki.osdev.org/Programmable_Interval_Timer
#define PIT_FREQUENCY 1193182 // magic intell number
#define PIT_CHANNEL_0 0x40
#define PIT_COMMAND 0x43


#define PIT_SELECT_CHANNEL_0 (0b00 << 6)
#define PIT_ACCESS_HI_LOW (0b11 << 4)
#define PIT_MODE_SQUARE_WAVE (0b011 << 1)

void pit_set_timer(uint16_t hertz);