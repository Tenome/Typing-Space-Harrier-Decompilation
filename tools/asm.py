"""dis.py <hex addr> : disassemble a function of tsh.exe (size from functions.tsv)."""
import sys, csv, os
sys.path.insert(0, os.path.dirname(__file__))
from tshimg import EXE, Image
import capstone
img = Image(EXE)
inv = {int(r['address'], 16): int(r['size']) for r in csv.DictReader(open(os.path.join(os.path.dirname(__file__), '..', 'config', 'functions.tsv')), delimiter='\t')}
a = int(sys.argv[1], 16)
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
for i in md.disasm(img.read(a, inv.get(a, 64)), a):
    print('%08x  %-6s %s' % (i.address, i.mnemonic, i.op_str))
