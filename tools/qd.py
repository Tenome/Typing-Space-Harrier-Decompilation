import sys, re, os, difflib
sys.path.insert(0, os.path.dirname(__file__))
import compare_obj as c, capstone
img = c.Image(c.EXE)
inv = {int(r["address"], 16): int(r["size"]) for r in __import__('csv').DictReader(open(c.INVENTORY, encoding="utf-8"), delimiter="\t")}
def norm(s): return re.sub(r'0x[0-9a-f]{4,}', 'T', s)
def show(address, code, original, first_diff):
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    orig = img.read(address, inv.get(address, len(code)))
    a = [(i.address-address, f"{i.mnemonic} {i.op_str}") for i in md.disasm(bytes(code), address)]
    b = [(i.address-address, f"{i.mnemonic} {i.op_str}") for i in md.disasm(bytes(orig), address)]
    sm = difflib.SequenceMatcher(None, [norm(x[1]) for x in a], [norm(x[1]) for x in b], autojunk=False)
    print("  similarity %.1f%% (mine %d insns, orig %d)" % (100*sm.ratio(), len(a), len(b)))
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal': continue
        print("  --- mine +%x / orig +%x" % (a[i1][0] if i1 < len(a) else -1, b[j1][0] if j1 < len(b) else -1))
        for k in range(max(i2-i1, j2-j1)):
            l = "%5x %s" % a[i1+k] if i1+k < i2 else ""
            r = "%5x %s" % b[j1+k] if j1+k < j2 else ""
            print("    %-44s | %s" % (l, r))
c.show_diff = show
sys.argv = [sys.argv[0]] + sys.argv[1:]
c.main()
