#include "pic.h"
#include <stdint.h>
#include "utils.h"
void pic_remap(int offset1, int offset2)
{
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1); // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2); // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 1 << CASCADE_IRQ); // ICW3: tell Master PIC that there is a slave PIC at IRQ2
	io_wait();
	outb(PIC2_DATA, CASCADE_IRQ); // ICW3: tell Slave PIC its cascade identity
	io_wait();

	outb(PIC1_DATA, ICW4_8086); // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
}

void pic_mask(uint8_t mask1, uint8_t mask2)
{
	outb(PIC1_DATA, mask1);
	outb(PIC2_DATA, mask2);
}

void pic_send_eoi(uint8_t irq)
{
	if (irq >= 8)
	{
		outb(PIC2_COMMAND, PIC_EOI);
	}
	outb(PIC1_COMMAND, PIC_EOI);
}