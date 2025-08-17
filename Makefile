# Makefile for HumbleOS (Auto-detect source files)

# Settings
BUILD_DIR := build
ISO_DIR := iso
SRC_DIR := src
INCLUDE_DIR := $(SRC_DIR)/include
CPP_INCLUDE_DIR := $(INCLUDE_DIR)/cpp

# Debug mode - set DEBUG=1 to enable
ifdef DEBUG
    DEBUG_FLAGS := -DDEBUG -g -O0
    STRIP_CMD := @echo "Debug mode: not stripping symbols"
else
    DEBUG_FLAGS := -O2
    STRIP_CMD := $(STRIP) $@
endif

GCC := $(HOME)/opt/cross/bin/i686-elf-gcc
G++ := $(HOME)/opt/cross/bin/i686-elf-g++
STRIP := $(HOME)/opt/cross/bin/i686-elf-strip
NASM := nasm
GRUB_MKRESCUE := grub-mkrescue

KERNEL := kernel.bin
LD_FILE := linker.ld
LINK_ARGS := -T $(LD_FILE) -o $(ISO_DIR)/boot/$(KERNEL) -ffreestanding -nostdlib -z noexecstack -no-pie -static $(DEBUG_FLAGS)
GCC_ARGS := -ffreestanding -Wall -Wextra -c -static -nostartfiles -I$(INCLUDE_DIR) -fno-pic -fno-pie \
	-Wno-unused-parameter -Wno-write-strings -fno-omit-frame-pointer $(DEBUG_FLAGS)
G++_ARGS := $(GCC_ARGS) -fno-exceptions -fno-rtti -Wno-sized-deallocation -I$(CPP_INCLUDE_DIR)
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
$(BUILD_DIR)/c/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/settings.h
	@mkdir -p $(dir $@)
	$(GCC) $(GCC_ARGS) -o $@ $<

# Assemble .asm files
$(BUILD_DIR)/asm/%.o: $(SRC_DIR)/%.asm $(SRC_DIR)/settings.h
	@mkdir -p $(dir $@)
	$(NASM) -felf32 $< -o $@

# Compile .cpp files
$(BUILD_DIR)/cpp/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/settings.h
	@mkdir -p $(dir $@)
	$(G++) $(G++_ARGS) $< -o $@

# Link kernel
$(ISO_DIR)/boot/$(KERNEL): $(OBJS)
	@mkdir -p $(dir $@)
	# Create empty symbols file if it doesn't exist
	@test -f src/symbols_generated.h || echo '{0, ""}' > src/symbols_generated.h
	$(STRIP_CMD)
ifdef DEBUG
	$(GCC) $(LINK_ARGS) $^ -lgcc
	bash generate_symbols.sh
	# Rebuild panic.o with new symbols
	$(GCC) $(GCC_ARGS) -o $(BUILD_DIR)/c/lib/panic.o src/lib/panic.c
	$(GCC) $(LINK_ARGS) $(OBJS) -lgcc
	$(STRIP_CMD)
endif
	$(GCC) $(LINK_ARGS) $^ -lgcc

# Create ISO
$(BUILD_DIR)/os.iso: $(ISO_DIR)/boot/$(KERNEL)
	$(GRUB_MKRESCUE) -o $@ $(ISO_DIR) -quiet

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)/boot/$(KERNEL) src/symbols_generated.h

# Run the OS
run: $(BUILD_DIR)/os.iso
	qemu-system-i386 -cdrom $(BUILD_DIR)/os.iso -boot d -m 128M -serial stdio
