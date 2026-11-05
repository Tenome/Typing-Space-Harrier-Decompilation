import os, struct
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
EXE = os.environ.get("TSH_EXE") or os.path.join(ROOT, "orig", "tsh.exe")
class Image:
    def __init__(self, path):
        self.data = open(path, "rb").read()
        d = self.data
        pe = struct.unpack_from("<I", d, 0x3C)[0]
        nsec = struct.unpack_from("<H", d, pe + 6)[0]
        opt_size = struct.unpack_from("<H", d, pe + 20)[0]
        self.base = struct.unpack_from("<I", d, pe + 24 + 28)[0]
        iat_rva, iat_size = struct.unpack_from("<II", d, pe + 24 + 96 + 12 * 8)
        self.iat = (self.base + iat_rva, self.base + iat_rva + iat_size)
        self.sections = {}
        off = pe + 24 + opt_size
        for _ in range(nsec):
            name = d[off:off + 8].rstrip(b"\0").decode()
            vsize, va, rsize, raw = struct.unpack_from("<IIII", d, off + 8)
            self.sections[name] = (self.base + va, vsize, raw, rsize)
            off += 40
        start, vsize, _, _ = self.sections[".text"]
        self.text = (start, start + vsize)

    def read(self, va, size):
        for start, vsize, raw, rsize in self.sections.values():
            if start <= va < start + rsize:
                o = raw + va - start
                return self.data[o:o + min(size, start + rsize - va)]
        return b""

    def in_text(self, va):
        return self.text[0] <= va < self.text[1]


