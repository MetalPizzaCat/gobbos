.PHONY = build

ELF_OUTPUT_DIR := output

BUILD_DIR := build
SOURCE_DIR := .

BOOT_FILES_DIR := boot/

CC := clang
CFLAGS := -Wall \
    -Wextra \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fno-PIC \
    -ffunction-sections \
    -fdata-sections \
    -m64 \
    -march=x86-64 \
    -mabi=sysv \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone \
    -mcmodel=kernel \
    -target x86_64-unknown-none-elf


OBJ_FILES :=  $(BUILD_DIR)/utils.asm.o $(BUILD_DIR)/main.c.o $(BUILD_DIR)/keyboard.c.o 

elf-image:
	rm -rf $(ELF_OUTPUT_DIR)/
	mkdir $(ELF_OUTPUT_DIR)/
	dd if=/dev/zero of=$(ELF_OUTPUT_DIR)/efi-image bs=1M count=64
	mkfs.fat -F32 $(ELF_OUTPUT_DIR)/efi-image
	mmd -i $(ELF_OUTPUT_DIR)/efi-image ::/EFI
	mmd -i $(ELF_OUTPUT_DIR)/efi-image ::/EFI/BOOT
	mcopy -i $(ELF_OUTPUT_DIR)/efi-image ./limine.conf ::/
	mcopy -i $(ELF_OUTPUT_DIR)/efi-image $(BOOT_FILES_DIR)/BOOTX64.EFI ::/EFI/BOOT

$(BUILD_DIR)/%.asm.o: $(SOURCE_DIR)/%.asm
	fasm $< $@

$(BUILD_DIR)/%.c.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BOOT_FILES_DIR)/kernel: $(OBJ_FILES)
	ld -T linker.lds $(OBJ_FILES) -o $(BOOT_FILES_DIR)/kernel -nostdlib 

build: elf-image $(BOOT_FILES_DIR)/kernel
	mcopy -i $(ELF_OUTPUT_DIR)/efi-image $(BOOT_FILES_DIR)/kernel ::/


run:
	qemu-system-x86_64 \
    -m 512M \
    -bios /usr/share/ovmf/OVMF.fd \
    -drive format=raw,file=$(ELF_OUTPUT_DIR)/efi-image