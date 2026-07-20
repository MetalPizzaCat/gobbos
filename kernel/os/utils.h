#pragma once

#include <stdint.h>

typedef struct
{
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) TableRegister; // global/interrupt descriptor table register

extern "C" void outb(uint16_t port, uint8_t byte);
extern "C" uint8_t inb(uint16_t port);
extern "C" void io_wait();
extern "C" void enable_interrupts();
extern "C" void disable_interrupts();
extern "C" void load_gdt(TableRegister *gdtr);
extern "C" void load_idt(TableRegister *idtr);
extern "C" void set_data_segment(uint16_t ds);
extern "C" void set_code_segment(uint16_t cs);