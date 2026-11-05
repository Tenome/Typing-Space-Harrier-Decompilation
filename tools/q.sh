#!/bin/sh
# q.sh File.cpp : gen game.h, apply build/Q/overrides.txt (old<TAB>new lines), compile, full diff
export VC6_DIR="E:\Microsoft Visual Studio"
export MSYS2_ARG_CONV_EXCL="*"
PY=/c/Users/Michael/AppData/Local/Programs/Python/Python313/python.exe
$PY tools/gen_game_h.py >/dev/null 2>&1
$PY - <<'P'
import os
p='include/game.h'; s=open(p).read()
if os.path.exists('build/Q/overrides.txt'):
    for l in open('build/Q/overrides.txt'):
        l=l.rstrip('\n')
        if '\t' in l:
            a,b=l.split('\t',1); s=s.replace(a,b)
open(p,'w').write(s)
P
mkdir -p build/obj
V=$(cygpath -u "$VC6_DIR")
PATH="$V/VC98/Bin:$V/Common/MSDev98/Bin:$PATH" INCLUDE="include;$VC6_DIR\VC98\Include" LIB="$VC6_DIR\VC98\Lib" cl /nologo /c /O2 /D WIN32 /D NDEBUG /D _WINDOWS $QFLAGS /Fobuild/obj/ src/$1 | grep -v "^$1$"
$PY tools/qd.py build/obj/${1%.cpp}.obj --verbose | grep -v "reloc +"
