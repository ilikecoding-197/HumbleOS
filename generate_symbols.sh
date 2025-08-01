#!/bin/bash
nm iso/boot/kernel.bin | grep ' T ' | sort -k1,1 | awk '{print $1 " " $3}' | while read addr name; do
    demangled=$(echo "$name" | c++filt)
    echo "{0x$addr, \"$demangled\"},"
done > src/symbols_generated.h
echo "{0, \"\"}" >> src/symbols_generated.h
