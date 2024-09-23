BUILD_DIR="build"
ISO_DIR="iso"
SRC_DIR="src"
LIB_DIR="$SRC_DIR/lib"
ASM_FILES=("multiboot_header" "boot" "idt_asm")
C_FILES=("main" "gdt" "console" "port" "idt" "pic" "exception_handlers" "keyboard")
GCC_ARGS="-ffreestanding -mgeneral-regs-only -Wall -Wextra -m32 -c -static -nostartfiles -I$LIB_DIR"
LD_INPUT=""
KERNEL="kernel.bin"
LD_FILE="linker.ld"
LD_ARGS="--nmagic --output=$ISO_DIR/boot/$KERNEL --script=$LD_FILE -melf_i386"


mkdir -p $BUILD_DIR
mkdir -p $ISO_DIR
rm -rf $BUILD_DIR/*

for i in "${ASM_FILES[@]}"
do
   LD_INPUT="$LD_INPUT $BUILD_DIR/$i.o"
   nasm -o $BUILD_DIR/$i.o $SRC_DIR/$i.asm -felf32
done

for i in "${C_FILES[@]}"
do
   LD_INPUT="$LD_INPUT $BUILD_DIR/$i.o"
   gcc -o $BUILD_DIR/$i.o $SRC_DIR/$i.c $GCC_ARGS
done

ld $LD_ARGS $LD_INPUT
grub-mkrescue -o build/os.iso $ISO_DIR
