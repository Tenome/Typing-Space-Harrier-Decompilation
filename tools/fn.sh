#!/bin/sh
# fn.sh <hex addr> : print Ghidra's decompilation of one function
awk -v pat="==== FUN_$1 @" 'index($0,pat){f=1} f{print} f&&/^}/{exit}' "$(dirname "$0")/../build/ghidra/export/tsh.exe/decompiled.c"
