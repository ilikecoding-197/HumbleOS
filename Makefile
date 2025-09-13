#
# Makefile - main makefile
#
# Part of HumbleOS
#
# Copyright 2025 Thomas Shrader
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software
# and associated documentation files (the \"Software\"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute,
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial
# portions of the Software.
# 
# THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
# NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

# settings
BUILD_DIR := build
ISO_DIR := iso
SRC_DIR := src
INCLUDE_DIR := $(SRC_DIR)/include
CPP_INCLUDE_DIR := $(INCLUDE_DIR)/cpp
KERNEL := kernel.bin
LD_FILE := linker.ld
CROSS_PREFIX := $(HOME)/opt/cross/bin/i686-elf-
GCC := $(CROSS_PREFIX)gcc
G++ := $(CROSS_PREFIX)g++
STRIP := $(CROSS_PREFIX)strip
NASM := nasm
GRUB_MKRESCUE := grub-mkrescue
PYTHON := python3
LINK_ARGS := -T $(LD_FILE) -o $(ISO_DIR)/boot/$(KERNEL) -ffreestanding -nostdlib -z noexecstack -no-pie -static $(DEBUG_FLAGS)
GCC_ARGS := -ffreestanding -Wall -Wextra -c -static -nostartfiles -I$(INCLUDE_DIR) -fno-pic -fno-pie \
	-Wno-unused-parameter -Wno-write-strings -fno-omit-frame-pointer $(DEBUG_FLAGS) -Wno-sign-compare
G++_ARGS := $(GCC_ARGS) -fno-exceptions -fno-rtti -Wno-sized-deallocation -I$(CPP_INCLUDE_DIR)

# debug mode - set DEBUG=1 to enable
ifdef DEBUG
	DEBUG_FLAGS := -DDEBUG -g -O0
	STRIP_CMD := @echo "debug mode: not stripping symbols"
else
	DEBUG_FLAGS := -O2
	STRIP_CMD := $(STRIP) $(ISO_DIR)/boot/$(KERNEL)
endif

# auto-discover source files
C_SRC := $(shell find $(SRC_DIR) -name '*.c')
ASM_SRC := $(shell find $(SRC_DIR) -name '*.asm')
CPP_SRC := $(shell find $(SRC_DIR) -name '*.cpp')

C_OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/c/%.o, $(C_SRC))
ASM_OBJS := $(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/asm/%.o, $(ASM_SRC))
CPP_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/cpp/%.o, $(CPP_SRC))
OBJS := $(C_OBJS) $(ASM_OBJS) $(CPP_OBJS)

.PHONY: all clean iso run defconfig menuconfig genconfig

all: $(BUILD_DIR)/os.iso

#
# ensure generated headers exist before compiling objects
#
$(SRC_DIR)/config.h:
	@echo "generating config.h"
	$(PYTHON) config/create_config_h.py

# compile .c files
$(BUILD_DIR)/c/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/config.h
	@mkdir -p $(dir $@)
	$(GCC) $(GCC_ARGS) -o $@ $<

# assemble .asm files
$(BUILD_DIR)/asm/%.o: $(SRC_DIR)/%.asm $(SRC_DIR)/config.h
	@mkdir -p $(dir $@)
	$(NASM) -f elf32 -o $@ $<

# compile .cpp files
$(BUILD_DIR)/cpp/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/config.h
	@mkdir -p $(dir $@)
	$(G++) $(G++_ARGS) -o $@ $<

# ensure symbols_generated.h exists (some scripts expect it)
$(shell test -f src/symbols_generated.h || echo '{0, ""}' > src/symbols_generated.h)

# link kernel (single, deterministic step)
$(ISO_DIR)/boot/$(KERNEL): $(SRC_DIR)/config.h $(OBJS) $(LD_FILE)
	@mkdir -p $(dir $@)
	$(GCC) $(LINK_ARGS) $(OBJS) -lgcc
	$(STRIP_CMD)

# create ISO
$(BUILD_DIR)/os.iso: $(ISO_DIR)/boot/$(KERNEL)
	$(GRUB_MKRESCUE) -o $@ $(ISO_DIR)

# clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)/boot/$(KERNEL) src/symbols_generated.h

# run the OS
run: $(BUILD_DIR)/os.iso
	qemu-system-i386 -cdrom $(BUILD_DIR)/os.iso -boot d -m 128M -serial stdio $(QEMUFLAGS) \
		-audiodev pa,id=speaker -machine pcspk-audiodev=speaker

# generate default config
defconfig:
	$(PYTHON) config/default_config.py

# change the config (menu)
menuconfig:
	$(PYTHON) config/config_editor.py

# force generate config.h
genconfig:
	$(PYTHON) config/create_config_h.py
