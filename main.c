#include "limine.h"
#include <stddef.h>
#include "utils.h"
#include "pic.h"
#include "pit.h"
#include "keyboard.h"

__attribute__((section(".limine_requests"))) struct limine_framebuffer_request fb_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST_ID,
};

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

typedef struct
{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t attributes;
	uint8_t flags_limit;
	uint8_t base_high;
} __attribute__((packed)) UserDescriptor;

typedef struct
{
	uint16_t target_offset_low;
	uint16_t target_selector;
	uint8_t ist; // 3 bits long, rest reserved
	uint8_t flags;
	uint16_t target_offset_mid;
	uint32_t target_offset_high;
	uint32_t reserved;
} __attribute__((packed)) InterruptGate;

__attribute__((aligned(16))) UserDescriptor gdt[3] = {
	{0}, // null descriptor
	{
		.limit_low = 0xffff,
		.base_low = 0,
		.base_mid = 0,
		.attributes = GDT_CODE_ATTRIBUTES,
		.flags_limit = 0xf | GDT_CODE_FLAGS,
		.base_high = 0,
	},
	{
		.limit_low = 0xffff,
		.base_low = 0,
		.base_mid = 0,
		.attributes = GDT_DATA_ATTRIBUTES,
		.flags_limit = 0xf | GDT_DATA_FLAGS,
		.base_high = 0,
	}};

__attribute__((aligned(16))) InterruptGate idt[64] = {0};

// Halt and catch fire function.
static void hcf(void)
{
	for (;;)
	{
#if defined(__x86_64__)
		asm("hlt");
#elif defined(__aarch64__) || defined(__riscv)
		asm("wfi");
#elif defined(__loongarch64)
		asm("idle 0");
#endif
	}
}

typedef struct
{
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} InterruptFrame;

void set_fb(uint8_t value)
{
	struct limine_framebuffer *framebuffer = fb_request.response->framebuffers[0];
	for (size_t y = 0; y < framebuffer->height; y++)
	{
		for (size_t x = 0; x < framebuffer->width * (framebuffer->bpp / 8); x++)
		{
			((uint8_t *)framebuffer->address)[x + y * framebuffer->pitch] = value;
		}
	}
}

__attribute__((interrupt)) void timer_handler(InterruptFrame *frame)
{
	static uint32_t ms;
	ms += 10;
	if (ms > 5000)
	{
		ms -= 5000;
	}
	else
	{
		pic_send_eoi(0);
		return;
	}

	// if (*(uint8_t *)fb_request.response->framebuffers[0]->address)
	// {
	// 	set_fb(0x00);
	// }
	// else
	// {
	// 	set_fb(0xff);
	// }
	pic_send_eoi(0);
}

__attribute__((interrupt)) void keyboard_handler(InterruptFrame *frame)
{
	uint8_t scancode = inb(0x60);
	if (scancode & (1 << 7))
	{
		set_fb(scancode & 0b01111111);
	}
	else
	{
		set_fb(0xff);
	}

	pic_send_eoi(1);
}

void irq7_15_spurious(struct interrupt_frame *frame)
{
	pic_send_eoi(0);
	return;
}

void kmain()
{
	TableRegister gdtr = {
		.limit = sizeof(gdt) - 1,
		.base = (uint64_t)gdt,
	};

	load_gdt(&gdtr);
	set_code_segment(0x8);
	set_data_segment(0x10);

	TableRegister idtr = {
		.limit = sizeof(idt) - 1,
		.base = (uint64_t)idt,
	};

	disable_interrupts();

	// we mask all the interrupts from the pic, because we can... ig you don't have to because we did disable interrupts
	pic_mask(0xff, 0xff);
	pic_remap(32, 40);

	/// pic will trigger it's interrupt 0(which we previously set to idt[32])
	pit_set_timer(100);

	idt[32] = (InterruptGate)make_interrupt(0x8, timer_handler);
	idt[33] = (InterruptGate)make_interrupt(0x8, keyboard_handler);
	idt[32 + 7] = (InterruptGate)make_interrupt(0x8, irq7_15_spurious);
	idt[32 + 15] = (InterruptGate)make_interrupt(0x8, irq7_15_spurious);

	load_idt(&idtr);

	uint16_t pic_mask1 = 0xffff;
	pic_mask1 ^= (1 << 0);
	pic_mask1 ^= (1 << 1);
	pic_mask1 ^= (1 << 7);
	pic_mask1 ^= (1 << 15);

	pic_mask(pic_mask1 & 0x00ff, (pic_mask1 & 0xff00) >> 16);
	enable_interrupts();

	for (;;)
		;
	hcf();
}