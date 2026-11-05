r"""Typing Space Harrier word banks (WORD\word_NN.dat).

The game reads each file whole; if it does not start with CR LF it XORs every byte with
KEY[i % len(KEY)] ^ 0xA7 (FUN_004051b0). A file that starts with CR LF is used as plain text, so
custom banks need no encryption. Layout after decoding: CR LF, then lines "reading<TAB>display"
(cp932), blank line at the end. The loader (FUN_0040c780) splits on control characters and takes
tokens in pairs (reading, display).

  tsh_words.py decode <WORD dir> <out dir>      word_NN.dat -> word_NN.txt (cp932 text)
  tsh_words.py encode <in.txt> <out.dat> [--xor] text -> plain .dat (default) or obfuscated like the original
"""
import sys, os

KEY = b"God's in His Heaven, all's right with the world."

def xor(b):
    return bytes(c ^ KEY[i % len(KEY)] ^ 0xA7 for i, c in enumerate(b))

def decode_bytes(b):
    return b if b[:2] == b"\r\n" else xor(b)

def encode_text(text, obfuscate=False):
    t = text.replace("\r\n", "\n").replace("\n", "\r\n")
    if not t.startswith("\r\n"):
        t = "\r\n" + t
    if not t.endswith("\r\n\r\n"):
        t = t.rstrip("\r\n") + "\r\n\r\n"
    b = t.encode("cp932")
    return xor(b) if obfuscate else b

def main(a):
    if len(a) >= 3 and a[0] == "decode":
        os.makedirs(a[2], exist_ok=True)
        for n in sorted(os.listdir(a[1])):
            if n.lower().endswith(".dat"):
                data = decode_bytes(open(os.path.join(a[1], n), "rb").read())
                open(os.path.join(a[2], n[:-4] + ".txt"), "wb").write(data)
    elif len(a) >= 3 and a[0] == "encode":
        text = open(a[1], "rb").read().decode("cp932")
        open(a[2], "wb").write(encode_text(text, "--xor" in a))
    else:
        print(__doc__)

if __name__ == "__main__":
    main(sys.argv[1:])
