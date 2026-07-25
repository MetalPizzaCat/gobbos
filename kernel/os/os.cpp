#include "os.hpp"
#include <os/heap.hpp>
#include <graphics/graphics.hpp>
#include "serial.hpp"
__attribute__((interrupt)) void timer_handler(InterruptFrame *frame)
{
	static uint32_t ms;
	ms += UPDATE_TIME;
	if (GameManager::getInstance().getUpdateHandler() != nullptr)
	{
		GameManager::getInstance().getUpdateHandler()();
	}
	// update_game_logic();
	if (ms > UPDATE_TIME * 1000)
	{
		ms -= UPDATE_TIME * 1000;
	}
	pic_send_eoi(0);
	return;
}

__attribute__((interrupt)) void keyboard_handler(InterruptFrame *frame)
{

	using namespace os::keyboard;
	uint8_t scancode = inb(0x60);

	GlobalKeyboard::getInstance().setPreviousKeyState(scancodeToKeycode(scancode), GlobalKeyboard::getInstance().isKeyPressed(scancodeToKeycode(scancode)));
	GlobalKeyboard::getInstance().setKeyState(scancodeToKeycode(scancode), !(bool)(scancode & 0b10000000));
	if (GameManager::getInstance().getUpdateHandler() != nullptr)
	{
		GameManager::getInstance().getUpdateHandler()();
	}
	pic_send_eoi(1);
}

__attribute__((interrupt)) void mouse_handler(InterruptFrame *frame)
{
	// if (!(inb(0x64) & 0b1))
	// {
	// 	pic_send_eoi(1);
	// 	return;
	// }

	uint8_t data = inb(0x60);
	uint32_t mouse_x = inb(0x60);
	uint32_t mouse_y = inb(0x60);

	// bits 5 and 4 tell the sign of the delta value
	if (data & 0b00010000)
	{
		mouse_y |= 0xFFFFFF00;
	}
	if (data & 0b00001000)
	{
		mouse_x |= 0xFFFFFF00;
	}
	// g_mouse.x += mouse_x;
	// g_mouse.y += mouse_y;

	pic_send_eoi(1);
}

__attribute__((interrupt)) void irq7_15_spurious(InterruptFrame *frame)
{
	pic_send_eoi(0);
	return;
}

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

void panic_with_message(const char *msg)
{
	os::graphics::Graphics::getInstance().clearScreen(os::graphics::Color(0, 255, 0));
	// TODO: Add message display
	panic();
}

void panic()
{
	hcf();
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
	// pit_set_timer(100);
	pit_set_timer(30);

	idt[32 + 0] = (InterruptGate)make_interrupt(0x8, timer_handler);
	idt[32 + 1] = (InterruptGate)make_interrupt(0x8, keyboard_handler);
	idt[32 + 7] = (InterruptGate)make_interrupt(0x8, irq7_15_spurious);
	idt[32 + 12] = (InterruptGate)make_interrupt(0x8, mouse_handler);
	idt[32 + 15] = (InterruptGate)make_interrupt(0x8, irq7_15_spurious);

	load_idt(&idtr);

	uint16_t pic_mask1 = 0xffff;
	pic_mask1 ^= (1 << 0);
	pic_mask1 ^= (1 << 1);
	pic_mask1 ^= (1 << 7);
	pic_mask1 ^= (1 << 12);
	pic_mask1 ^= (1 << 15);

	pic_mask(pic_mask1 & 0x00ff, (pic_mask1 & 0xff00) >> 16);
	enable_interrupts();

	os::serial::Serial::getInstance().init();
	os::serial::Serial::getInstance().clear();
	os::serial::Serial::getInstance().writeCString("Goblins are the best!");

	os::memory::HeapInfo::getInstance().initLargestHeap();
	main();
	hcf();
}