import struct, sys, hashlib
d = open(sys.argv[1], 'rb').read()
print('sha1', hashlib.sha1(d).hexdigest(), 'size', len(d))
pe = struct.unpack_from('<I', d, 0x3c)[0]
(machine, nsec, ts, _, _, optsz, chars) = struct.unpack_from('<HHIIIHH', d, pe+4)
print('machine %x nsec %d timestamp %#x chars %#x' % (machine, nsec, ts, chars))
o = pe + 24
(magic, lmaj, lmin) = struct.unpack_from('<HBB', d, o)
print('linker %d.%02d' % (lmaj, lmin))
ep, base = struct.unpack_from('<I', d, o+16)[0], struct.unpack_from('<I', d, o+28)[0]
print('entry %#x base %#x' % (ep, base))
sa, fa = struct.unpack_from('<II', d, o+32)
print('secalign %#x filealign %#x' % (sa, fa))
sub = struct.unpack_from('<H', d, o+68)[0]; print('subsystem', sub)
nd = struct.unpack_from('<I', d, o+92)[0]
dirs = struct.unpack_from('<%dI' % (nd*2), d, o+96)
names = 'export import resource exception security reloc debug arch gp tls loadcfg boundimp iat delay clr res'.split()
for i in range(nd):
    if dirs[2*i] or dirs[2*i+1]: print(' dir', names[i] if i < len(names) else i, hex(dirs[2*i]), hex(dirs[2*i+1]))
so = o + optsz
for i in range(nsec):
    n, vs, va, rs, ro = struct.unpack_from('<8sIIII', d, so+40*i)
    print(' sec', n.rstrip(b'\0').decode(), hex(va), hex(vs), 'raw', hex(ro), hex(rs))
# Rich header
r = d.find(b'Rich')
if r > 0:
    key = struct.unpack_from('<I', d, r+4)[0]
    s = r
    while s > 0x80:
        if struct.unpack_from('<I', d, s-4)[0] ^ key == 0x536e6144 and False: pass
        s -= 4
        if (struct.unpack_from('<I', d, s)[0] ^ key) == 0x536e6144: break
    print('Rich entries:')
    p = s + 16
    while p < r:
        a, b = struct.unpack_from('<II', d, p); a ^= key; b ^= key
        print('  prodid %#x build %d count %d' % (a >> 16, a & 0xffff, b))
        p += 8
else:
    print('no Rich header')
