"""Minimal C/C++ statement parser and printer for permuter mutations (function bodies only)."""
import re

KEYWORDS_NOT_TYPE = {"return", "goto", "delete", "new", "else", "case", "sizeof", "break", "continue", "default", "do", "throw"}
TOK_RE = re.compile(
    r'\s+|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|0[xX][0-9a-fA-F]+[uUlL]*|\d+(?:\.\d+)?[fFuUlL]*|[A-Za-z_]\w*'
    r'|->|\+\+|--|<<=|>>=|<<|>>|<=|>=|==|!=|&&|\|\||[-+*/%&|^]=|::|.', re.S)


def strip_comments(s):
    out = []
    i = 0
    n = len(s)
    while i < n:
        c = s[i]
        if c == '"' or c == "'":
            j = i + 1
            while j < n and s[j] != c:
                j += 2 if s[j] == "\\" else 1
            out.append(s[i:j + 1])
            i = j + 1
        elif s.startswith("//", i):
            while i < n and s[i] != "\n":
                i += 1
        elif s.startswith("/*", i):
            j = s.find("*/", i + 2)
            i = n if j < 0 else j + 2
            out.append(" ")
        else:
            out.append(c)
            i += 1
    return "".join(out)


def tokenize(s):
    return [t for t in TOK_RE.findall(s) if not t.isspace()]


IDENT_RE = re.compile(r"^[A-Za-z_]\w*$")
NUM_RE = re.compile(r"^(0[xX][0-9a-fA-F]+|\d+)[uUlL]*$")
KW_OPERAND = {"return", "case", "else", "sizeof", "new", "delete", "throw"}


def is_operand_end(t):
    return t is not None and (t in (")", "]") or NUM_RE.match(t) or t.startswith('"') or t.startswith("'")
                              or (IDENT_RE.match(t) and t not in KW_OPERAND))


def render(toks):
    out = []
    prev = None
    prev2 = None
    for k, t in enumerate(toks):
        nxt = toks[k + 1] if k + 1 < len(toks) else None
        binary = is_operand_end(prev)
        if t in ("*", "&") and prev == ")" and False:
            binary = False
        sp = ""
        if prev is None:
            sp = ""
        elif t in (")", "]", ",", ";", ".", "->", "++", "--") and not (t in ("++", "--") and not binary):
            sp = ""
        elif prev in ("(", "[", ".", "->", "!", "~", "::") or t == "::":
            sp = ""
        elif t in ("(",):
            sp = " " if (prev in ("if", "while", "for", "switch", "return", "case") or prev in ("=", "+", "-", "*", "/", "&&", "||", "==", "!=", "<", ">", "<=", ">=", "?", ":", "&", "|", "^", "%", "<<", ">>", ",") or (prev[-1:] == "=" and len(prev) > 1)) else ""
        elif t == "[":
            sp = ""
        elif prev in ("++", "--") and not is_operand_end(prev2):
            sp = ""
        elif prev in ("-", "+", "*", "&") and not is_operand_end(prev2):
            sp = ""
        elif t in ("++", "--"):
            sp = ""
        else:
            sp = " "
        out.append(sp + t)
        prev2, prev = prev, t
    return "".join(out)


def norm(s):
    return render(tokenize(s))


# ---------------- nodes ----------------

class Node:
    kind = "node"


class Simple(Node):
    kind = "simple"

    def __init__(self, text):
        self.text = text


class Label(Node):
    kind = "label"

    def __init__(self, text):
        self.text = text


class Block(Node):
    kind = "block"

    def __init__(self, items=None, braced=True):
        self.items = items if items is not None else []
        self.braced = braced


class If(Node):
    kind = "if"

    def __init__(self, cond, then, els=None):
        self.cond, self.then, self.els = cond, then, els


class For(Node):
    kind = "for"

    def __init__(self, init, cond, inc, body):
        self.init, self.cond, self.inc, self.body = init, cond, inc, body


class While(Node):
    kind = "while"

    def __init__(self, cond, body):
        self.cond, self.body = cond, body


class Do(Node):
    kind = "do"

    def __init__(self, body, cond):
        self.body, self.cond = body, cond


class Switch(Node):
    kind = "switch"

    def __init__(self, expr, body):
        self.expr, self.body = expr, body


class Parser:
    def __init__(self, s):
        self.s = s
        self.i = 0

    def ws(self):
        s = self.s
        while self.i < len(s) and s[self.i].isspace():
            self.i += 1

    def peek_word(self):
        m = re.compile(r"[A-Za-z_]\w*").match(self.s, self.i)
        return m.group(0) if m else None

    def read_group(self, open_c, close_c):
        s = self.s
        assert s[self.i] == open_c, (open_c, s[self.i:self.i + 30])
        depth = 0
        j = self.i
        while j < len(s):
            c = s[j]
            if c == '"' or c == "'":
                k = j + 1
                while s[k] != c:
                    k += 2 if s[k] == "\\" else 1
                j = k
            elif c == open_c:
                depth += 1
            elif c == close_c:
                depth -= 1
                if depth == 0:
                    inner = s[self.i + 1:j]
                    self.i = j + 1
                    return inner
            j += 1
        raise ValueError("unbalanced")

    def read_until_semi(self):
        s = self.s
        depth = 0
        j = self.i
        while j < len(s):
            c = s[j]
            if c == '"' or c == "'":
                k = j + 1
                while s[k] != c:
                    k += 2 if s[k] == "\\" else 1
                j = k
            elif c in "([{":
                depth += 1
            elif c in ")]}":
                depth -= 1
            elif c == ";" and depth == 0:
                text = s[self.i:j]
                self.i = j + 1
                return text
            j += 1
        raise ValueError("no semicolon")

    def body(self):
        self.ws()
        if self.s[self.i] == "{":
            return self.block()
        st = self.statement()
        return Block([st] if st is not None else [], braced=False)

    def block(self):
        inner = self.read_group("{", "}")
        p = Parser(inner)
        items = []
        while True:
            p.ws()
            if p.i >= len(inner):
                break
            st = p.statement()
            if st is not None:
                items.append(st)
        return Block(items, True)

    def statement(self):
        self.ws()
        s = self.s
        c = s[self.i]
        if c == ";":
            self.i += 1
            return None
        if c == "{":
            return self.block()
        w = self.peek_word()
        if w == "if":
            self.i += 2
            self.ws()
            cond = norm(self.read_group("(", ")"))
            then = self.body()
            self.ws()
            els = None
            if self.peek_word() == "else":
                self.i += 4
                self.ws()
                if self.peek_word() == "if":
                    els = Block([self.statement()], braced=False)
                else:
                    els = self.body()
            return If(cond, then, els)
        if w == "for":
            self.i += 3
            self.ws()
            inner = self.read_group("(", ")")
            parts = split_top(inner, ";")
            assert len(parts) == 3, inner
            return For(norm(parts[0]), norm(parts[1]), norm(parts[2]), self.body())
        if w == "while":
            self.i += 5
            self.ws()
            cond = norm(self.read_group("(", ")"))
            return While(cond, self.body())
        if w == "do":
            self.i += 2
            body = self.body()
            self.ws()
            assert self.peek_word() == "while"
            self.i += 5
            self.ws()
            cond = norm(self.read_group("(", ")"))
            self.ws()
            if self.s[self.i] == ";":
                self.i += 1
            return Do(body, cond)
        if w == "switch":
            self.i += 6
            self.ws()
            expr = norm(self.read_group("(", ")"))
            self.ws()
            body = self.block()
            return Switch(expr, body)
        if w in ("case", "default"):
            j = self.i
            depth = 0
            while True:
                ch = s[j]
                if ch == "?":
                    depth += 1
                elif ch == ":":
                    if depth == 0:
                        break
                    depth -= 1
                elif ch == "'":
                    j += 2 if s[j + 1] == "\\" else 1
                    j += 1
                j += 1
            text = s[self.i:j].strip()
            self.i = j + 1
            return Label(norm(text) + ":")
        m = re.compile(r"([A-Za-z_]\w*)\s*:(?!:)").match(s, self.i)
        if m and w not in ("case", "default"):
            self.i = m.end()
            self.ws()
            if self.i < len(s) and s[self.i] == ";":
                self.i += 1
                return Label(m.group(1) + ":;")
            return Label(m.group(1) + ":")
        text = self.read_until_semi()
        return Simple(norm(text))


def split_top(s, sep):
    parts = []
    depth = 0
    cur = []
    i = 0
    while i < len(s):
        c = s[i]
        if c == '"' or c == "'":
            j = i + 1
            while s[j] != c:
                j += 2 if s[j] == "\\" else 1
            cur.append(s[i:j + 1])
            i = j + 1
            continue
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        if c == sep and depth == 0:
            parts.append("".join(cur))
            cur = []
        else:
            cur.append(c)
        i += 1
    parts.append("".join(cur))
    return parts


# ---------------- printing ----------------

def pr_body(b, ind, out, force_brace=False):
    """Print a Block used as a statement body; returns nothing. Caller already printed header (without brace/newline)."""
    if b.braced or len(b.items) != 1 or force_brace or b.items[0].kind in ("label",):
        out[-1] += " {"
        for it in b.items:
            pr_node(it, ind + 1, out)
        out.append("    " * ind + "}")
        return True
    out.append("")
    out.pop()
    pr_node(b.items[0], ind + 1, out)
    return False


def pr_node(n, ind, out):
    pad = "    " * ind
    k = n.kind
    if k == "simple":
        out.append(pad + n.text + ";")
    elif k == "label":
        out.append("    " * max(ind - 1, 0) + n.text if n.text.startswith(("case", "default")) else n.text)
    elif k == "block":
        out.append(pad + "{")
        for it in n.items:
            pr_node(it, ind + 1, out)
        out.append(pad + "}")
    elif k == "if":
        out.append(pad + "if (" + n.cond + ")")
        braced = pr_body(n.then, ind, out, force_brace=bool(n.els) and _needs_brace(n))
        if n.els is not None:
            if len(n.els.items) == 1 and n.els.items[0].kind == "if" and not n.els.braced:
                sub = []
                pr_node(n.els.items[0], ind, sub)
                sub[0] = sub[0].lstrip()
                if braced:
                    out[-1] += " else " + sub[0][0:]
                else:
                    out.append(pad + "else " + sub[0])
                out.extend(sub[1:])
            else:
                if braced:
                    out[-1] += " else"
                else:
                    out.append(pad + "else")
                pr_body(n.els, ind, out)
    elif k == "for":
        out.append(pad + "for (" + n.init + "; " + n.cond + "; " + n.inc + ")")
        pr_body(n.body, ind, out)
    elif k == "while":
        out.append(pad + "while (" + n.cond + ")")
        pr_body(n.body, ind, out)
    elif k == "do":
        out.append(pad + "do")
        out[-1] += " {"
        for it in n.body.items:
            pr_node(it, ind + 1, out)
        out.append(pad + "} while (" + n.cond + ");")
    elif k == "switch":
        out.append(pad + "switch (" + n.expr + ") {")
        for it in n.body.items:
            pr_node(it, ind + 1, out)
        out.append(pad + "}")


def _needs_brace(n):
    t = n.then
    return (not t.braced) and len(t.items) == 1 and t.items[0].kind in ("if", "for", "while")


def print_body(block):
    out = []
    for it in block.items:
        pr_node(it, 1, out)
    return "\n".join(out)


# ---------------- function extraction ----------------

FUNC_HEAD_RE = re.compile(r"^[A-Za-z_].*\)\s*$")


def split_functions(text):
    """Return list of segments: ('other', text) or ('func', header_lines, body_text, trailer) in file order."""
    lines = text.split("\n")
    segs = []
    i = 0
    cur_other = []
    while i < len(lines):
        ln = lines[i]
        if (FUNC_HEAD_RE.match(ln) and not ln.startswith(("typedef", "extern", "struct ", "class ", "#"))
                and "(" in ln and i + 1 < len(lines) and lines[i + 1] == "{"):
            j = i + 2
            while j < len(lines) and lines[j] != "}":
                j += 1
            # attach preceding comment lines
            cm = []
            while cur_other and cur_other[-1].startswith("//"):
                cm.insert(0, cur_other.pop())
            if cur_other:
                segs.append(("other", "\n".join(cur_other)))
            cur_other = []
            segs.append(("func", cm, ln, "\n".join(lines[i + 2:j])))
            i = j + 1
            continue
        cur_other.append(ln)
        i += 1
    if cur_other:
        segs.append(("other", "\n".join(cur_other)))
    return segs


def parse_function_body(body_text):
    p = Parser("{" + strip_comments(body_text) + "}")
    return p.block()
