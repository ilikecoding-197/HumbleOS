BUILD_DIR="build"
ISO_DIR="iso"
SRC_DIR="src"
INCLUDE_DIR="$SRC_DIR/include"
ASM_FILES=( \
	"boot/multiboot_header" \
	"boot/boot" \
	"interrupts/idt" \
)
C_FILES=( \
	"main" \
	"boot/gdt" \
	"lib/console" \
	"lib/port" \
	"interrupts/idt" \
	"lib/pic" \
	"interrupts/exception_handlers" \
	"lib/keyboard" \
)
GCC_ARGS="-ffreestanding -mgeneral-regs-only -Wall -Wextra -m32 -c -static -nostartfiles -I$INCLUDE_DIR"
LD_INPUT=""
KERNEL="kernel.bin"
LD_FILE="linker.ld"
LD_ARGS="--nmagic --output=$ISO_DIR/boot/$KERNEL --script=$LD_FILE -melf_i386"


mkdir -p $BUILD_DIR
mkdir -p $ISO_DIR
rm -rf $BUILD_DIR/*
mkdir -p $BUILD_DIR/asm
mkdir -p $BUILD_DIR/c

for i in "${ASM_FILES[@]}"
do
   LD_INPUT="$LD_INPUT $BUILD_DIR/asm/$i.o"
   mkdir -p $BUILD_DIR/asm/$(dirname $i)
   nasm -o $BUILD_DIR/asm/$i.o $SRC_DIR/$i.asm -felf32
done

for i in "${C_FILES[@]}"
do
   LD_INPUT="$LD_INPUT $BUILD_DIR/c/$i.o"
   mkdir -p $BUILD_DIR/c/$(dirname $i)
   gcc -o $BUILD_DIR/c/$i.o $SRC_DIR/$i.c $GCC_ARGS
done

ld $LD_ARGS $LD_INPUT
grub-mkrescue -o build/os.iso $ISO_DIR
