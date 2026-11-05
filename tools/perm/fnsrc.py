"""Extract a target function from a lane source file and assemble a standalone scratch source."""
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import cparse as P  # noqa: E402

DECOMP = os.path.abspath(os.path.join(HERE, "..", ".."))


def find_file(addr_hex):
    pat = re.compile(r"(?:M|FUN)_%s\(" % addr_hex.lower().zfill(8))
    for fn in sorted(os.listdir(os.path.join(DECOMP, "src"))):
        if not fn.endswith(".cpp") or fn.startswith("M_"):
            continue
        t = open(os.path.join(DECOMP, "src", fn), encoding="latin-1").read()
        for seg in P.split_functions(t):
            if seg[0] == "func" and pat.search(seg[2]) and not seg[2].rstrip().endswith(";"):
                return fn
    return None


class Target:
    def __init__(self, file, addr_hex, keep_others=False):
        self.file = file
        self.addr_hex = addr_hex.lower().zfill(8)
        text = open(os.path.join(DECOMP, "src", file), encoding="latin-1").read()
        segs = P.split_functions(text)
        pre = []
        self.header = None
        self.comment = []
        self.post = []
        found = False
        for seg in segs:
            if seg[0] == "other":
                (self.post if found else pre).append(seg[1])
            else:
                _, cm, head, body = seg
                if re.search(r"(?:M|FUN)_%s\(" % self.addr_hex, head) and not found:
                    found = True
                    self.comment, self.header, self.body = cm, head, body
                elif keep_others or head.lstrip().startswith("static"):
                    (self.post if found else pre).append("\n".join(cm + [head, "{", body, "}"]))
        if not found:
            raise SystemExit("function %s not found in %s" % (addr_hex, file))
        self.prefix = "\n".join(pre) + "\n"
        self.suffix = "\n".join(self.post)
        self.tree = P.parse_function_body(self.body)

    def source_for(self, body_text):
        return self.prefix + "\n" + self.header + "\n{\n" + body_text + "\n}\n" + self.suffix + "\n"

    def raw_source(self):
        return self.source_for(self.body)

    def tree_body(self, tree):
        return P.print_body(tree)
