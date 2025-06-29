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
)

GCC_ARGS="-ffreestanding -Wall -Wextra -m32 -c -static -nostartfiles -I$INCLUDE_DIR -nostdinc" # Args for GCC
LD_INPUT="" # Input for LD
KERNEL="kernel.bin" # Kernel output file
LD_FILE="linker.ld" # Linker script
LD_ARGS="--nmagic --output=$ISO_DIR/boot/$KERNEL --script=$LD_FILE -melf_i386" # Args for LD


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
   echo gcc -o $BUILD_DIR/c/$i.o $SRC_DIR/$i.c $GCC_ARGS
   gcc -o $BUILD_DIR/c/$i.o $SRC_DIR/$i.c $GCC_ARGS
done

# Link
echo "LD..."
echo ld $LD_ARGS $LD_INPUT
ld $LD_ARGS $LD_INPUT

# Strip
echo "STRIP..."
echo strip $ISO_DIR/boot/$KERNEL
strip $ISO_DIR/boot/$KERNEL

# Grub ISO
echo "ISO..."
echo grub-mkrescue -o build/os.iso $ISO_DIR
grub-mkrescue -o build/os.iso $ISO_DIR -quiet

echo "Compiling done."