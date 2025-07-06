# Build script for HumbleOS

# Settings
BUILD_DIR="build" # Our build directory (where the .o files will be)
ISO_DIR="iso" # Directory containing files for the output iso
SRC_DIR="src" # Source
INCLUDE_DIR="$SRC_DIR/include" # Where our incl,ude (.h files) are

# Our ASM files
ASM_FILES=( \
	"boot/multiboot_header" \
	"boot/boot" \
	"interrupts/idt" \
	"lib/panic" \
	"lib/time" \
	"lib/cpuid" \
)

# Our C files
C_FILES=( \
	"main" \
	"boot/gdt" \
	"lib/console" \
	"lib/port" \
	"interrupts/idt" \
	"lib/pic" \
	"interrupts/exception_handlers" \
	"lib/ps2_controller" \
	"lib/panic" \
	"lib/heap" \
	"lib/string" \
	"lib/time" \
	"lib/numconvert" \
	"lib/sys_info" \
	"lib/cpuid" \
	"lib/rand" \
	"lib/printf"
)

GCC_ARGS="-ffreestanding -Wall -Wextra -O2 -c -static -nostartfiles -I$INCLUDE_DIR -fno-pic -fno-pie" # Args for GCC
LD_INPUT="" # Input for LD
KERNEL="kernel.bin" # Kernel output file
LD_FILE="linker.ld" # Linker script
LINK_ARGS="-T linker.ld -o $ISO_DIR/boot/$KERNEL -ffreestanding -O2 -nostdlib -z noexecstack -no-pie -nostdlib -static" # Args for LD
GCC="$HOME/opt/cross/bin/i686-elf-gcc"

# Build DIR
mkdir -p $BUILD_DIR
mkdir -p $ISO_DIR
rm -rf $BUILD_DIR/*
mkdir -p $BUILD_DIR/asm
mkdir -p $BUILD_DIR/c

# Assemble ASM files
echo "ASM..."
for i in "${ASM_FILES[@]}"
do
   LD_INPUT="$LD_INPUT $BUILD_DIR/asm/$i.o"
   mkdir -p $BUILD_DIR/asm/$(dirname $i)
   echo nasm -o $BUILD_DIR/asm/$i.o $SRC_DIR/$i.asm -felf32
   nasm -o $BUILD_DIR/asm/$i.o $SRC_DIR/$i.asm -felf32
done

# Compile C files
echo "C..."
for i in "${C_FILES[@]}"
do
   LD_INPUT="$LD_INPUT $BUILD_DIR/c/$i.o"
   mkdir -p $BUILD_DIR/c/$(dirname $i)
   echo $GCC -o $BUILD_DIR/c/$i.o $SRC_DIR/$i.c $GCC_ARGS
   $GCC -o $BUILD_DIR/c/$i.o $SRC_DIR/$i.c $GCC_ARGS
done

# Link
echo "LINK..."
echo $GCC $LINK_ARGS $LD_INPUT
$GCC $LINK_ARGS $LD_INPUT -lgcc

# Strip
echo "STRIP..."
echo strip $ISO_DIR/boot/$KERNEL
strip $ISO_DIR/boot/$KERNEL

# Grub ISO
echo "ISO..."
echo grub-mkrescue -o build/os.iso $ISO_DIR
grub-mkrescue -o build/os.iso $ISO_DIR -quiet

echo "Compiling done."