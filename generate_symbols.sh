#!/bin/bash
echo "Generating symbols..." >&2
rm -rf src/symbols_generated.h
$HOME/opt/cross/bin/i686-elf-nm iso/boot/kernel.bin | grep ' T ' | sort -k1,1 | awk '{print $1 " " $3}' | while read addr name; do
    demangled=$(echo "$name" | $HOME/opt/cross/bin/i686-elf-c++filt)
    echo "{0x$addr, \"$demangled\"},"
done > src/symbols_generated.h
echo "{0, \"\"}" >> src/symbols_generated.h
echo "Generated $(wc -l < src/symbols_generated.h) symbol entries" >&2
