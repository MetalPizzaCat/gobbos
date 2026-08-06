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
public enable_sse
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

enable_sse:
	mov rax, cr0
	and ax, 0xFFFB		;clear coprocessor emulation CR0.EM
	or ax, 0x2			;set coprocessor monitoring  CR0.MP
	mov cr0, rax
	mov rax, cr4
	or ax, 1536 ;3 << 9		;set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
	mov cr4, rax
	ret