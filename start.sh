BUILD_DIR="build"
ISO_DIR="$BUILD_DIR/iso"
SRC_DIR="src"
ASM_FILES=("multiboot_header" "boot")
LD_INPUT=""
KERNEL="kernel.bin"
LD_FILE="linker.ld"
LD_ARGS="--nmagic --output=$ISO_DIR/boot/$KERNEL --script=$LD_FILE"

mkdir -p $BUILD_DIR

for i in "${ASM_FILES[@]}"
do
   LD_INPUT="$LD_INPUT $BUILD_DIR/$i.o"
   nasm -o $BUILD_DIR/$i.o $SRC_DIR/$i.asm -felf64
done

ld $LD_ARGS $LD_INPUT
grub-mkrescue -o build/os.iso $ISO_DIR