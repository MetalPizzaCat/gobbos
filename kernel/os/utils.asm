format ELF64

section '.text' executable

public outb
public inb
public io_wait
public enable_interrupts
public disable_interrupts
public load_gdt
public load_idt
public set_data_segment
public set_code_segment

; output byte 
outb:
    mov dx, di
    mov al, sil
    out dx, al
    ret

; read input byte
inb:
    mov dx, di
    in al, dx
    ret

io_wait:
    mov ax, 0
    out 0x80, ax
    ret

enable_interrupts:
    sti
    ret

disable_interrupts:
    cli
    ret

load_gdt:
    lgdt [rdi]
    ret

load_idt:
    lidt [rdi]
    ret

set_data_segment:
    mov ax, di
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

set_code_segment:
    pop rax
    push rdi
    push rax
    retfq