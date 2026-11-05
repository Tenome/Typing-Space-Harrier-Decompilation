#!/bin/sh
export VC6_DIR="E:\Microsoft Visual Studio"
PY=/c/Users/Michael/AppData/Local/Programs/Python/Python313/python.exe
$PY tools/build_all.py --file $1 2>&1 | grep -i "error\|FAILED\|C[0-9][0-9][0-9][0-9]" 
$PY tools/qd.py build/obj/${1%.cpp}.obj --verbose | grep -v "reloc +"
