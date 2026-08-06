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

__attribute__((interrupt)) void exceptionDivideByZeroError(InterruptFrame *frame) { panic_with_message("Attempted to divide by zero"); }
__attribute__((interrupt)) void exceptionDebug(InterruptFrame *frame) { panic_with_message("Debug exception"); }
__attribute__((interrupt)) void exceptionNonMaskableInterrupt(InterruptFrame *frame) { panic_with_message("Non maskable interrupt"); }
__attribute__((interrupt)) void exceptionBreakpoint(InterruptFrame *frame) { panic_with_message("Breakpoint triggered"); }
__attribute__((interrupt)) void exceptionOverflow(InterruptFrame *frame) { panic_with_message("Overflow error"); }
__attribute__((interrupt)) void exceptionBoundRange(InterruptFrame *frame) { panic_with_message("Range error"); }
__attribute__((interrupt)) void exceptionInvalidOpcode(InterruptFrame *frame) { panic_with_message("Invalid opcode"); }
__attribute__((interrupt)) void exceptionDeviceNotAvailable(InterruptFrame *frame) { panic_with_message("Device not available"); }
__attribute__((interrupt)) void exceptionDoubleFault(InterruptFrame *frame) { panic_with_message("Double fault"); }
__attribute__((interrupt)) void exceptionInvalidTss(InterruptFrame *frame) { panic_with_message("Invalid task state segment"); }
__attribute__((interrupt)) void exceptionSegmentNotPresent(InterruptFrame *frame) { panic_with_message("Segment not present"); }
__attribute__((interrupt)) void exceptionStack(InterruptFrame *frame) { panic_with_message("Stack error"); }
__attribute__((interrupt)) void exceptionGeneralProtection(InterruptFrame *frame) { panic_with_message("General protection"); }
__attribute__((interrupt)) void exceptionPageFault(InterruptFrame *frame) { panic_with_message("Page fault"); }
__attribute__((interrupt)) void exception87FloatingPointExceptionPending(InterruptFrame *frame) { panic_with_message("x87 Floating point exception"); }
__attribute__((interrupt)) void exceptionAlignmentCheck(InterruptFrame *frame) { panic_with_message("Misaligned memory"); }
__attribute__((interrupt)) void exceptionMachineCheck(InterruptFrame *frame) { panic_with_message("Machine check"); }
__attribute__((interrupt)) void exceptionSimdFloatingPoint(InterruptFrame *frame) { panic_with_message("SIMD floating point"); }
__attribute__((interrupt)) void exceptionControlProtectionException(InterruptFrame *frame) { panic_with_message("Control protection"); }
__attribute__((interrupt)) void exceptionHypervisorInjectionException(InterruptFrame *frame) { panic_with_message("Hypervisor injection"); }
__attribute__((interrupt)) void exceptionVmmCommunicationException(InterruptFrame *frame) { panic_with_message("VMM Communication event"); }
__attribute__((interrupt)) void exceptionSecurityException(InterruptFrame *frame) { panic_with_message("Host security exception"); }

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
	os::serial::Serial::getInstance().writeCString(msg);
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

	idt[0] = (InterruptGate)make_interrupt(0x8, exceptionDivideByZeroError);
	idt[1] = (InterruptGate)make_interrupt(0x8, exceptionDebug);
	idt[2] = (InterruptGate)make_interrupt(0x8, exceptionNonMaskableInterrupt);
	idt[3] = (InterruptGate)make_interrupt(0x8, exceptionBreakpoint);
	idt[4] = (InterruptGate)make_interrupt(0x8, exceptionOverflow);
	idt[5] = (InterruptGate)make_interrupt(0x8, exceptionBoundRange);
	idt[6] = (InterruptGate)make_interrupt(0x8, exceptionInvalidOpcode);
	idt[7] = (InterruptGate)make_interrupt(0x8, exceptionDeviceNotAvailable);
	idt[8] = (InterruptGate)make_interrupt(0x8, exceptionDoubleFault);
	idt[10] = (InterruptGate)make_interrupt(0x8, exceptionInvalidTss);
	idt[11] = (InterruptGate)make_interrupt(0x8, exceptionSegmentNotPresent);
	idt[12] = (InterruptGate)make_interrupt(0x8, exceptionStack);
	idt[13] = (InterruptGate)make_interrupt(0x8, exceptionGeneralProtection);
	idt[14] = (InterruptGate)make_interrupt(0x8, exceptionPageFault);
	idt[16] = (InterruptGate)make_interrupt(0x8, exception87FloatingPointExceptionPending);
	idt[17] = (InterruptGate)make_interrupt(0x8, exceptionAlignmentCheck);
	idt[18] = (InterruptGate)make_interrupt(0x8, exceptionMachineCheck);
	idt[19] = (InterruptGate)make_interrupt(0x8, exceptionSimdFloatingPoint);
	idt[21] = (InterruptGate)make_interrupt(0x8, exceptionControlProtectionException);
	idt[28] = (InterruptGate)make_interrupt(0x8, exceptionHypervisorInjectionException);
	idt[29] = (InterruptGate)make_interrupt(0x8, exceptionVmmCommunicationException);
	idt[30] = (InterruptGate)make_interrupt(0x8, exceptionSecurityException);

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

	enable_sse();

	os::serial::Serial::getInstance().init();
	os::serial::Serial::getInstance().clear();
	os::serial::Serial::getInstance().writeCString("Goblins are the best!");

	os::memory::HeapInfo::getInstance().initLargestHeap();
	main();
	hcf();
}