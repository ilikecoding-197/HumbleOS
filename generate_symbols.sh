#
# generate_symbols.sh - generate symbols
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

#!/bin/bash
echo "Generating symbols..." >&2
rm -rf src/symbols_generated.h
$HOME/opt/cross/bin/i686-elf-nm iso/boot/kernel.bin | grep ' T ' | sort -k1,1 | awk '{print $1 " " $3}' | while read addr name; do
    demangled=$(echo "$name" | $HOME/opt/cross/bin/i686-elf-c++filt)
    echo "{0x$addr, \"$demangled\"},"
done > src/symbols_generated.h
echo "{0, \"\"}" >> src/symbols_generated.h
echo "Generated $(wc -l < src/symbols_generated.h) symbol entries" >&2
