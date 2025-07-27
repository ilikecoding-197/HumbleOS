# Makefile for HumbleOS (Auto-detect source files)

# Settings
BUILD_DIR := build
ISO_DIR := iso
SRC_DIR := src
INCLUDE_DIR := $(SRC_DIR)/include

GCC := $(HOME)/opt/cross/bin/i686-elf-gcc
G++ := $(HOME)/opt/cross/bin/i686-elf-g++
STRIP := $(HOME)/opt/cross/bin/i686-elf-strip
NASM := nasm
GRUB_MKRESCUE := grub-mkrescue

KERNEL := kernel.bin
LD_FILE := linker.ld
LINK_ARGS := -T $(LD_FILE) -o $(ISO_DIR)/boot/$(KERNEL) -ffreestanding -O2 -nostdlib -z noexecstack -no-pie -static
GCC_ARGS := -ffreestanding -Wall -Wextra -O2 -c -static -nostartfiles -I$(INCLUDE_DIR) -fno-pic -fno-pie
G++_ARGS := $(GCC_ARGS) -fno-exceptions -fno-rtti -Wno-write-strings -Wno-sized-deallocation

# Auto-discover source files
C_SRC := $(shell find $(SRC_DIR) -name '*.c')
ASM_SRC := $(shell find $(SRC_DIR) -name '*.asm')
CPP_SRC := $(shell find $(SRC_DIR) -name '*.cpp')

C_OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/c/%.o, $(C_SRC))
ASM_OBJS := $(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/asm/%.o, $(ASM_SRC))
CPP_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/cpp/%.o, $(CPP_SRC))
OBJS := $(C_OBJS) $(ASM_OBJS) $(CPP_OBJS)

.PHONY: all clean iso run

all: $(BUILD_DIR)/os.iso

# Compile .c files
$(BUILD_DIR)/c/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(GCC) $(GCC_ARGS) -o $@ $<

# Assemble .asm files
$(BUILD_DIR)/asm/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(NASM) -felf32 $< -o $@

# Compile .cpp files
$(BUILD_DIR)/cpp/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(G++) $(G++_ARGS) $< -o $@

# Link kernel
$(ISO_DIR)/boot/$(KERNEL): $(OBJS)
	@mkdir -p $(dir $@)
	$(GCC) $(LINK_ARGS) $^ -lgcc
	$(STRIP) $@

# Create ISO
$(BUILD_DIR)/os.iso: $(ISO_DIR)/boot/$(KERNEL)
	$(GRUB_MKRESCUE) -o $@ $(ISO_DIR) -quiet

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)/boot/$(KERNEL)

# Run the OS
run: $(BUILD_DIR)/os.iso
	qemu-system-i386 -cdrom $(BUILD_DIR)/os.iso -boot d -m 128M