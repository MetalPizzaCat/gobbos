#pragma once
#ifndef OS_HEADER
#define OS_HEADER
#include "limine.h"
#include <stddef.h>
#include "utils.h"
#include "pic.hpp"
#include "pit.hpp"
#include <keyboard/keyboard.hpp>
#include <memory.hpp>
#include <stdbool.h>
#include "game.hpp"

#define GDT_ATTRIBUTE_PRESENT (1 << 7)
#define GDT_ATTRIBUTE_USER (1 << 4)					  // 1 for user descriptors (code and data), 0 for system descriptors and call gates
#define GDT_ATTRIBUTE_CODE (1 << 3)					  // 1 for a code segment, 0 for a data segment
#define GDT_ATTRIBUTE_CONFORMING_EXPAND_DOWN (1 << 2) // set conforming for code segments, and expand down for data segments
#define GDT_ATTRIBUTE_READ_WRITE (1 << 1)			  // set readable for code segements and writeable for data segments
#define GDT_ATTRIBUTE_ACCESSED (1 << 0)

#define GDT_FLAG_GRANULARITY (1 << 7)
#define GDT_FLAG_OPERAND_SIZE (1 << 6)
#define GDT_FLAG_LONG_MODE (1 << 5)
#define GDT_FLAG_AVAILABLE_TO_SOFTWARE (1 << 4)

#define GDT_CODE_ATTRIBUTES (GDT_ATTRIBUTE_PRESENT | GDT_ATTRIBUTE_CODE | GDT_ATTRIBUTE_USER | GDT_ATTRIBUTE_READ_WRITE)
#define GDT_CODE_FLAGS (GDT_FLAG_LONG_MODE | GDT_FLAG_GRANULARITY)

#define GDT_DATA_ATTRIBUTES (GDT_ATTRIBUTE_PRESENT | GDT_ATTRIBUTE_USER | GDT_ATTRIBUTE_READ_WRITE)
#define GDT_DATA_FLAGS (GDT_FLAG_GRANULARITY)

#define INTERRUPT_GATE_TYPE 0xE
#define INTERRUPT_GATE_PRESENT (1 << 7)

#define make_interrupt(cs, address)                                    \
	{                                                                  \
		.target_offset_low = (uint16_t)((uint64_t)(address)),          \
		.target_selector = (cs),                                       \
		.ist = 0,                                                      \
		.flags = INTERRUPT_GATE_TYPE | INTERRUPT_GATE_PRESENT,         \
		.target_offset_mid = (uint16_t)(((uint64_t)(address)) >> 16),  \
		.target_offset_high = (uint32_t)(((uint64_t)(address)) >> 32), \
		.reserved = 0,                                                 \
	}

#define TIMER_FREQUENCY 100
#define UPDATE_TIME 1 / TIMER_FREQUENCY

__attribute__((packed)) struct UserDescriptor
{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t attributes;
	uint8_t flags_limit;
	uint8_t base_high;
};

__attribute__((packed)) typedef struct InterruptGate
{
	uint16_t target_offset_low;
	uint16_t target_selector;
	uint8_t ist; // 3 bits long, rest reserved
	uint8_t flags;
	uint16_t target_offset_mid;
	uint32_t target_offset_high;
	uint32_t reserved;
};

using GameHandlerFunctionType = void (*)(void);

class GameManager
{
public:
	static GameManager &getInstance()
	{
		static GameManager manager;
		return manager;
	}

	GameHandlerFunctionType getUpdateHandler() const
	{
		return this->m_updateHandler;
	}
	GameHandlerFunctionType getInputHandler() const
	{

		return this->m_inputHandler;
	}

	void setInputHandler(GameHandlerFunctionType handler)
	{
		m_inputHandler = handler;
	}

	void setUpdateHandler(GameHandlerFunctionType handler)
	{
		m_updateHandler = handler;
	}

private:
	GameHandlerFunctionType m_updateHandler = nullptr;
	GameHandlerFunctionType m_inputHandler = nullptr;
};

// Halt and catch fire function.
static void hcf(void)
{
	for (;;)
	{
		asm("hlt");
	}
}
struct InterruptFrame
{
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
};

/// Display error message and halt all execution. A green screen will be displayed
/// @param msg Message to display
extern "C" void panic_with_message(const char *msg);

extern "C" void panic();

extern "C" __attribute__((interrupt)) void timer_handler(InterruptFrame *frame);

extern "C" __attribute__((interrupt)) void keyboard_handler(InterruptFrame *frame);

extern "C" __attribute__((interrupt)) void mouse_handler(InterruptFrame *frame);

extern "C" __attribute__((interrupt)) void irq7_15_spurious(InterruptFrame *frame);

void main();

/// The actual main function of the os, however for the sake of simplicity i chose to hide and provide a more standard main
/// This is meant to setup all the internal logic while main should be dealing with game specific setups and menus ig?
extern "C" void kmain();

#endif