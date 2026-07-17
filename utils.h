#pragma once

#include <stddef.h>

typedef struct
{
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) TableRegister; // global/interrupt descriptor table register

void outb(uint16_t port, uint8_t byte);
uint8_t inb(uint16_t port);
void io_wait();
void enable_interrupts();
void disable_interrupts();
void load_gdt(TableRegister *gdtr);
void load_idt(TableRegister *idtr);
void set_data_segment(uint16_t ds);
void set_code_segment(uint16_t cs);