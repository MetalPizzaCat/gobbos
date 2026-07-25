.PHONY = build

ELF_OUTPUT_DIR := output

BUILD_DIR := build
SOURCE_DIR := .

BOOT_FILES_DIR := boot/


STD_DIR := std/

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
    -target x86_64-unknown-none-elf \
	-fno-threadsafe-statics \
	-fno-exceptions \
	-g \
	-I $(STD_DIR) \
	-I kernel


OBJ_FILES :=  $(BUILD_DIR)/kernel/os/utils.asm.o \
					$(BUILD_DIR)/kernel/main.cpp.o \
					$(BUILD_DIR)/kernel/os/os.cpp.o \
					$(BUILD_DIR)/kernel/os/serial.cpp.o \
					$(BUILD_DIR)/kernel/os/io.cpp.o \
					$(BUILD_DIR)/kernel/os/heap.cpp.o \
					$(BUILD_DIR)/kernel/keyboard/keyboard.cpp.o \
					$(BUILD_DIR)/kernel/os/pit.cpp.o \
					$(BUILD_DIR)/kernel/os/pic.cpp.o \
					$(BUILD_DIR)/kernel/graphics/graphics.cpp.o \
					$(BUILD_DIR)/std/memory.cpp.o \
					$(BUILD_DIR)/games/invaders/invaders.cpp.o \
					$(BUILD_DIR)/std/typeinfo.cpp.o \
					$(BUILD_DIR)/std/new.cpp.o \
					$(BUILD_DIR)/std/exception.cpp.o \



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

$(BUILD_DIR)/%.cpp.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<



$(BOOT_FILES_DIR)/kernel: $(OBJ_FILES)
	ld -T linker.lds $(OBJ_FILES) -o $(BOOT_FILES_DIR)/kernel -nostdlib 

paths:
	mkdir -p ./$(BUILD_DIR)/kernel/os
	mkdir -p ./$(BUILD_DIR)/kernel/keyboard
	mkdir -p ./$(BUILD_DIR)/kernel/graphics
	mkdir -p ./$(BUILD_DIR)/std
	mkdir -p ./$(BUILD_DIR)/games
	mkdir -p ./$(BUILD_DIR)/games/invaders

build: paths elf-image $(BOOT_FILES_DIR)/kernel
	mcopy -i $(ELF_OUTPUT_DIR)/efi-image $(BOOT_FILES_DIR)/kernel ::/


run:
	qemu-system-x86_64 \
    -m 512M \
    -bios /usr/share/ovmf/OVMF.fd \
    -drive format=raw,file=$(ELF_OUTPUT_DIR)/efi-image


debug:
	qemu-system-x86_64 \
    -m 512M \
    -bios /usr/share/ovmf/OVMF.fd \
    -drive format=raw,file=$(ELF_OUTPUT_DIR)/efi-image -s -S

clean:
	@rm -rf $(BUILD_DIR)