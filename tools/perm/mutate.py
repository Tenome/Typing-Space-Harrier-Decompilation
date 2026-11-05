"""Semantics-preserving (by heuristic) mutations on a cparse tree."""
import copy
import random
import re

import cparse as P
from cparse import Simple, Label, Block, If, For, While, Do, Switch, tokenize, render, IDENT_RE, NUM_RE

ASSIGN_OPS = {"=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>="}
CMP_FLIP = {"<": ">", ">": "<", "<=": ">=", ">=": "<=", "==": "==", "!=": "!="}
CMP_NEG = {"<": ">=", ">": "<=", "<=": ">", ">=": "<", "==": "!=", "!=": "=="}
NOT_NAMES = {"return", "break", "continue", "goto", "case", "default", "if", "else", "while", "for", "do", "switch",
             "sizeof", "int", "char", "short", "long", "unsigned", "signed", "void", "struct", "float", "double"}
TYPE_WORDS = {"int", "char", "short", "long", "unsigned", "signed", "float", "double", "void", "const", "struct", "BOOL",
              "DWORD", "WORD", "BYTE", "enum", "static", "register"}


def is_mem_name(t):
    return t.startswith("m_") or t.startswith("DAT_")


# ---------------- text analysis ----------------

class Info:
    def __init__(self):
        self.reads = set()
        self.writes = set()
        self.calls = False
        self.memw = False
        self.memr = False
        self.flow = False

    def merge(self, o):
        self.reads |= o.reads
        self.writes |= o.writes
        self.calls |= o.calls
        self.memw |= o.memw
        self.memr |= o.memr
        self.flow |= o.flow


def lvalue_start(toks, k):
    """index of first token of the postfix chain ending just before toks[k]"""
    j = k - 1
    while j >= 0:
        t = toks[j]
        if t in (")", "]"):
            close = t
            openc = "(" if t == ")" else "["
            d = 0
            while j >= 0:
                if toks[j] == close:
                    d += 1
                elif toks[j] == openc:
                    d -= 1
                    if d == 0:
                        break
                j -= 1
            j -= 1
            continue
        if IDENT_RE.match(t) or NUM_RE.match(t):
            if j >= 1 and toks[j - 1] in (".", "->"):
                j -= 2
                continue
            if j >= 1 and toks[j - 1] == "*" and (j < 2 or not P.is_operand_end(toks[j - 2])):
                return j - 1
            return j
        break
    return j + 1


def text_info(text):
    toks = tokenize(text)
    inf = Info()
    n = len(toks)
    for k, t in enumerate(toks):
        nxt = toks[k + 1] if k + 1 < n else None
        prev = toks[k - 1] if k else None
        if IDENT_RE.match(t) and t not in NOT_NAMES and t not in TYPE_WORDS:
            if prev not in (".", "->"):
                loc = t
                j = k
                while not is_mem_name(t) and j + 2 < n and toks[j + 1] == "." and IDENT_RE.match(toks[j + 2]):
                    loc += "." + toks[j + 2]
                    j += 2
                inf.reads.add(loc)
                if is_mem_name(t):
                    inf.memr = True
            if nxt == "(" and t != "sizeof":
                inf.calls = True
        if t in ("->", "["):
            inf.memr = True
        if t == "*" and not P.is_operand_end(prev):
            inf.memr = True
        if t in ("return", "break", "continue", "goto"):
            inf.flow = True
        if t in ASSIGN_OPS or t in ("++", "--"):
            if t in ("++", "--") and not P.is_operand_end(prev):
                # prefix: operand follows
                j = k + 1
                chain = toks[j:j + 6]
                base = next((x for x in chain if IDENT_RE.match(x)), None)
                chain_txt = " ".join(chain)
            else:
                s = lvalue_start(toks, k)
                chain = toks[s:k]
                base = next((x for x in chain if IDENT_RE.match(x)), None)
            complex_lv = any(x in ("->", "[", "*", ".") for x in chain)
            dotted = bool(chain) and all(IDENT_RE.match(x) or x == "." for x in chain) and chain[0] != "." and                 all(chain[i] == "." for i in range(1, len(chain), 2)) and not is_mem_name(chain[0])
            if base:
                if dotted:
                    inf.writes.add("".join(chain))
                elif complex_lv or is_mem_name(base):
                    inf.memw = True
                else:
                    inf.writes.add(base)
        if t == "&" and not P.is_operand_end(prev) and nxt and IDENT_RE.match(nxt):
            inf.writes.add(nxt)
    return inf


def node_info(n):
    inf = Info()
    k = n.kind
    if k in ("simple",):
        inf.merge(text_info(n.text))
        d = parse_decl(n.text)
        if d:
            for dd in d[1]:
                inf.writes.add(dd[1])
    elif k == "label":
        inf.flow = True
    elif k == "block":
        for it in n.items:
            inf.merge(node_info(it))
    elif k == "if":
        inf.merge(text_info(n.cond))
        inf.merge(node_info(n.then))
        if n.els:
            inf.merge(node_info(n.els))
    elif k == "for":
        for t in (n.init, n.cond, n.inc):
            inf.merge(text_info(t))
        inf.merge(node_info(n.body))
    elif k in ("while", "do"):
        inf.merge(text_info(n.cond))
        inf.merge(node_info(n.body))
    elif k == "switch":
        inf.merge(text_info(n.expr))
        inf.merge(node_info(n.body))
    return inf


def _has_name(inf, name):
    for loc in inf.reads | inf.writes:
        if loc == name or loc.startswith(name + "."):
            return True
    return False


def _loc_overlap(ws, others):
    for w in ws:
        for o in others:
            if w == o or o.startswith(w + ".") or w.startswith(o + "."):
                return True
    return False


def conflict(a, b):
    ia, ib = node_info(a), node_info(b)
    if ia.flow or ib.flow:
        return True
    if _loc_overlap(ia.writes, ib.reads | ib.writes) or _loc_overlap(ib.writes, ia.reads):
        return True
    if ia.memw and (ib.memr or ib.memw or ib.calls):
        return True
    if ib.memw and (ia.memr or ia.calls):
        return True
    if ia.calls and (ib.calls or ib.memr):
        return True
    if ib.calls and ia.memr:
        return True
    return False


# ---------------- declarations ----------------

def split_commas(toks):
    parts, cur, d = [], [], 0
    for t in toks:
        if t in ("(", "[", "{"):
            d += 1
        elif t in (")", "]", "}"):
            d -= 1
        if t == "," and d == 0:
            parts.append(cur)
            cur = []
        else:
            cur.append(t)
    parts.append(cur)
    return parts


def parse_decl(text):
    """-> (type_tokens, [(stars, name, arr_tokens, init_tokens or None)]) or None"""
    toks = tokenize(text)
    if len(toks) < 2 or toks[0] in NOT_NAMES - TYPE_WORDS or toks[0] in KEYW_STMT:
        return None
    groups = split_commas(toks)
    g = groups[0]
    if "=" in g:
        e = g.index("=")
        head, init = g[:e], g[e + 1:]
    else:
        head, init = g, None
    arr = []
    while head and head[-1] == "]":
        d = 0
        j = len(head) - 1
        while j >= 0:
            if head[j] == "]":
                d += 1
            elif head[j] == "[":
                d -= 1
                if d == 0:
                    break
            j -= 1
        arr = head[j:] + arr
        head = head[:j]
    if not head or not IDENT_RE.match(head[-1]) or head[-1] in TYPE_WORDS:
        return None
    name = head[-1]
    head = head[:-1]
    stars = 0
    while head and head[-1] == "*":
        stars += 1
        head = head[:-1]
    if not head or not all(IDENT_RE.match(x) or x == "::" for x in head):
        return None
    if head[0] in KEYW_STMT:
        return None
    if len(head) == 1 and head[0] not in TYPE_WORDS and not (head[0][:1].isupper() or head[0].startswith("_")):
        # lower-case single identifier type like 'foo bar' is unusual
        return None
    decls = [("*" * stars, name, arr, init)]
    for g in groups[1:]:
        if "=" in g:
            e = g.index("=")
            h, i2 = g[:e], g[e + 1:]
        else:
            h, i2 = g, None
        a2 = []
        while h and h[-1] == "]":
            d = 0
            j = len(h) - 1
            while j >= 0:
                if h[j] == "]":
                    d += 1
                elif h[j] == "[":
                    d -= 1
                    if d == 0:
                        break
                j -= 1
            a2 = h[j:] + a2
            h = h[:j]
        st = 0
        while h and h[0] == "*":
            st += 1
            h = h[1:]
        if len(h) != 1 or not IDENT_RE.match(h[0]):
            return None
        decls.append(("*" * st, h[0], a2, i2))
    return head, decls


KEYW_STMT = {"return", "goto", "delete", "new", "else", "case", "default", "break", "continue", "throw", "sizeof"}


def make_decl(typ, decls):
    parts = []
    for stars, name, arr, init in decls:
        s = stars + name + "".join(arr)
        if init is not None:
            s += " = " + render(init)
        parts.append(s)
    first = " ".join(typ) + " " + parts[0]
    return Simple(first + "".join(", " + p for p in parts[1:]))


def is_decl(n):
    return n.kind == "simple" and parse_decl(n.text) is not None


# ---------------- tree walking ----------------

def blocks_of(tree):
    out = []

    def walk(n):
        if n.kind == "block":
            out.append(n)
            for it in n.items:
                walk(it)
        elif n.kind == "if":
            walk(n.then)
            if n.els:
                walk(n.els)
        elif n.kind in ("for", "while", "do"):
            walk(n.body)
        elif n.kind == "switch":
            walk(n.body)
    walk(tree)
    return out


def all_nodes(tree):
    out = []

    def walk(n):
        out.append(n)
        if n.kind == "block":
            for it in n.items:
                walk(it)
        elif n.kind == "if":
            walk(n.then)
            if n.els:
                walk(n.els)
        elif n.kind in ("for", "while", "do"):
            walk(n.body)
        elif n.kind == "switch":
            walk(n.body)
    walk(tree)
    return out


def text_slots(tree):
    """(node, attr) pairs holding expression text"""
    out = []
    for n in all_nodes(tree):
        if n.kind == "simple":
            out.append((n, "text"))
        elif n.kind in ("if", "while", "do"):
            out.append((n, "cond"))
        elif n.kind == "for":
            out += [(n, "init"), (n, "cond"), (n, "inc")]
        elif n.kind == "switch":
            out.append((n, "expr"))
    return out


def mentions(node, name):
    inf = node_info(node)
    return _has_name(inf, name)


def is_terminal(n):
    if n.kind == "simple":
        t = tokenize(n.text)
        return bool(t) and t[0] in ("return", "break", "continue", "goto")
    if n.kind == "block" and n.items:
        return is_terminal(n.items[-1])
    return False


# ---------------- expression helpers ----------------

def match_group(toks, k):
    """index of the token closing the bracket opened at toks[k]"""
    o = toks[k]
    c = {"(": ")", "[": "]"}[o]
    d = 0
    for j in range(k, len(toks)):
        if toks[j] == o:
            d += 1
        elif toks[j] == c:
            d -= 1
            if d == 0:
                return j
    return None


def is_cast(inner):
    return bool(inner) and all(IDENT_RE.match(t) or t == "*" for t in inner) and (
        inner[0] in TYPE_WORDS or inner[0][:1].isupper()) and inner[0] not in ("sizeof",)


def operand_right(toks, k):
    """extent [k, e) of a unary/postfix operand starting at k, or None"""
    j = k
    n = len(toks)
    while j < n:
        if toks[j] in ("!", "-", "~", "*", "&", "++", "--"):
            j += 1
        elif toks[j] == "(" and match_group(toks, j) is not None and is_cast(toks[j + 1:match_group(toks, j)]):
            j = match_group(toks, j) + 1
        else:
            break
    if j >= n:
        return None
    t = toks[j]
    if t == "(":
        e = match_group(toks, j)
        if e is None:
            return None
        # cast: (type) operand -> treat conservative: not an atom
        j = e + 1
    elif IDENT_RE.match(t) or NUM_RE.match(t) or t.startswith('"') or t.startswith("'"):
        j += 1
    else:
        return None
    while j < n:
        if toks[j] in (".", "->") and j + 1 < n and IDENT_RE.match(toks[j + 1]):
            j += 2
        elif toks[j] in ("[", "("):
            e = match_group(toks, j)
            if e is None:
                return None
            j = e + 1
        elif toks[j] in ("++", "--"):
            j += 1
        else:
            break
    return k, j


def operand_left(toks, k):
    """extent [s, k) of an operand ending just before k"""
    if k <= 0:
        return None
    s = lvalue_start(toks, k)
    if s >= k:
        return None
    # include casts and unary prefixes
    while s > 0 and toks[s - 1] == ")":
        o = s - 1
        d = 0
        while o >= 0:
            if toks[o] == ")":
                d += 1
            elif toks[o] == "(":
                d -= 1
                if d == 0:
                    break
            o -= 1
        if o >= 0 and is_cast(toks[o + 1:s - 1]) and not (o > 0 and IDENT_RE.match(toks[o - 1]) and toks[o - 1] not in P.KW_OPERAND):
            s = o
        else:
            break
    while s > 0 and toks[s - 1] in ("!", "-", "~") and not P.is_operand_end(toks[s - 2] if s >= 2 else None):
        s -= 1
    return s, k


LEFT_DELIMS = {"(", ",", "=", "&&", "||", "?", ":", "return", "==", "!=", "<", ">", "<=", ">=", None} | ASSIGN_OPS
RIGHT_DELIMS = {")", ",", ";", "&&", "||", "?", ":", "==", "!=", "<", ">", "<=", ">=", None, "]"}


def _tok(toks, k):
    return toks[k] if 0 <= k < len(toks) else None


def binary_sites(toks, ops, left_extra=(), right_extra=()):
    sites = []
    for k, t in enumerate(toks):
        if t not in ops or not P.is_operand_end(_tok(toks, k - 1)):
            continue
        L = operand_left(toks, k)
        R = operand_right(toks, k + 1)
        if not L or not R:
            continue
        if _tok(toks, L[0] - 1) not in LEFT_DELIMS | set(left_extra):
            continue
        if _tok(toks, R[1]) not in RIGHT_DELIMS | set(right_extra):
            continue
        sites.append((L, k, R))
    return sites


def is_pure_operand(seg):
    return not any(t in ("++", "--", "(", "=") or t in ASSIGN_OPS for t in seg)


def rule_flip_cmp(toks, rng):
    sites = binary_sites(toks, set(CMP_FLIP))
    if not sites:
        return None
    L, k, R = rng.choice(sites)
    a, b = toks[L[0]:L[1]], toks[R[0]:R[1]]
    return toks[:L[0]] + b + [CMP_FLIP[toks[k]]] + a + toks[R[1]:]


def rule_commute(toks, rng):
    sites = []
    for op, lx, rx in (("+", ("+",), ("+", "-")), ("*", ("*",), ("*",)), ("&", ("&",), ("&",)), ("|", ("|",), ("|",)),
                       ("^", ("^",), ("^",)), ("==", (), ()), ("!=", (), ())):
        for s in binary_sites(toks, {op}, lx, rx):
            sites.append(s)
    if not sites:
        return None
    L, k, R = rng.choice(sites)
    a, b = toks[L[0]:L[1]], toks[R[0]:R[1]]
    return toks[:L[0]] + b + [toks[k]] + a + toks[R[1]:]


def rule_commute_logic(toks, rng):
    sites = []
    for s in binary_sites(toks, {"&&", "||"}, ("&&", "||"), ("&&", "||")):
        L, k, R = s
        a, b = toks[L[0]:L[1]], toks[R[0]:R[1]]
        if toks[k] == "&&" and _tok(toks, L[0] - 1) == "||" or toks[k] == "||" and _tok(toks, L[0] - 1) == "&&":
            continue
        if is_pure_operand(a) and is_pure_operand(b):
            ia, ib = text_info(render(a)), text_info(render(b))
            if not (ia.reads & ib.reads - {"this"}) or True:
                if not (ia.writes & ib.reads):
                    sites.append(s)
    # comparison-level: operands are comparisons (atoms around cmp) handled by widening
    sites2 = []
    for k, t in enumerate(toks):
        if t in ("&&", "||"):
            # left side: back to previous && / || / ( at depth 0
            d = 0
            j = k - 1
            while j >= 0:
                if toks[j] in (")", "]"):
                    d += 1
                elif toks[j] in ("(", "["):
                    if d == 0:
                        break
                    d -= 1
                elif d == 0 and toks[j] in ("&&", "||", "?", ":", "=") :
                    break
                j -= 1
            ls = j + 1
            d = 0
            j = k + 1
            while j < len(toks):
                if toks[j] in ("(", "["):
                    d += 1
                elif toks[j] in (")", "]"):
                    if d == 0:
                        break
                    d -= 1
                elif d == 0 and toks[j] in ("&&", "||", "?", ":", ",", ";"):
                    break
                j += 1
            re_ = j
            if ls >= k or re_ <= k + 1:
                continue
            a, b = toks[ls:k], toks[k + 1:re_]
            lp = toks[ls - 1] if ls > 0 else None
            rp = toks[re_] if re_ < len(toks) else None
            if t == "&&" and (lp in ("||",) or rp in ("||",)):
                continue
            if t == "||" and (lp in ("&&",) or rp in ("&&",)):
                continue
            if lp in ("&&", "||") and lp != t or rp in ("&&", "||") and rp != t:
                continue
            if not (is_pure_operand_deref(a) and is_pure_operand_deref(b)):
                continue
            ia, ib = text_info(render(a)), text_info(render(b))
            if ia.reads & ib.reads - {"this"}:
                continue
            sites2.append((ls, k, re_))
    if sites2 and (not sites or rng.random() < 0.8):
        ls, k, re_ = rng.choice(sites2)
        return toks[:ls] + toks[k + 1:re_] + [toks[k]] + toks[ls:k] + toks[re_:]
    if not sites:
        return None
    L, k, R = rng.choice(sites)
    a, b = toks[L[0]:L[1]], toks[R[0]:R[1]]
    return toks[:L[0]] + b + [toks[k]] + a + toks[R[1]:]


def is_pure_operand_deref(seg):
    return not any(t in ("++", "--", "=", "?") or t in ASSIGN_OPS for t in seg) and not any(
        IDENT_RE.match(seg[i]) and i + 1 < len(seg) and seg[i + 1] == "(" and seg[i] not in ("sizeof",) for i in range(len(seg)))


def rule_incdec_form(toks, rng):
    if len(toks) == 2 and toks[1] in ("++", "--") and IDENT_RE.match(toks[0]):
        return [toks[1], toks[0]]
    if len(toks) == 2 and toks[0] in ("++", "--") and IDENT_RE.match(toks[1]):
        return [toks[1], toks[0]]
    # inside for increments: handled since the slot text is the whole inc expression
    sites = [k for k, t in enumerate(toks) if t in ("++", "--")]
    if not sites:
        return None
    k = rng.choice(sites)
    # only allowed when the value is unused: whole text is a statement (checked by caller via flag)
    return None


def rule_compound_form(toks, rng):
    # x op= e  <->  x = x op e   (x: plain identifier or simple member chain)
    if len(toks) >= 3 and toks[0] in ("++", "--") or (len(toks) == 2 and toks[1] in ("++", "--")):
        v = toks[1] if toks[0] in ("++", "--") else toks[0]
        op = toks[0] if toks[0] in ("++", "--") else toks[1]
        if not IDENT_RE.match(v):
            return None
        return rng.choice([[v, "+=" if op == "++" else "-=", "1"], [v, "=", v, "+" if op == "++" else "-", "1"]])
    for k, t in enumerate(toks):
        if t in ASSIGN_OPS and t != "=" and k >= 1 and is_pure_operand(toks[:k]):
            lhs = toks[:k]
            if _tok(toks, -1) is None:
                pass
            rhs = toks[k + 1:]
            if not rhs:
                return None
            op = t[:-1]
            if any(x in ASSIGN_OPS or x == "?" for x in rhs):
                return None
            if len(rhs) > 1 and op in ("+", "-", "*", "&", "|", "^", "<<", ">>", "/", "%"):
                rhs2 = ["("] + rhs + [")"] if any(x in ("+", "-", "|", "&", "^", "<<", ">>", "?", "&&", "||") for x in rhs) else rhs
            else:
                rhs2 = rhs
            if rhs == [] :
                return None
            return lhs + ["="] + lhs + [op] + rhs2
    # x = x op e
    if "=" in toks:
        k = toks.index("=")
        lhs = toks[:k]
        rhs = toks[k + 1:]
        n = len(lhs)
        if n >= 1 and is_pure_operand(lhs) and rhs[:n] == lhs and len(rhs) > n + 1 and rhs[n] in ("+", "-", "*", "&", "|", "^", "<<", ">>"):
            rest = rhs[n + 1:]
            if rest and not any(x in ("+", "-", "?", "&&", "||", "|", "&", "^", "<<", ">>", "==", "!=", "<", ">", "<=", ">=") for x in rest[1:]) or len(rest) == 1:
                return lhs + [rhs[n] + "="] + rest
    return None


def split_conj(toks):
    """top-level split by && / || -> (parts, ops) or None when mixed"""
    parts, ops, cur, d = [], [], [], 0
    for t in toks:
        if t in ("(", "["):
            d += 1
        elif t in (")", "]"):
            d -= 1
        if d == 0 and t in ("&&", "||"):
            parts.append(cur)
            ops.append(t)
            cur = []
        else:
            cur.append(t)
    parts.append(cur)
    return parts, ops


def rule_not_eq0(toks, rng):
    parts, ops = split_conj(toks)
    idx = list(range(len(parts)))
    rng.shuffle(idx)
    for i in idx:
        p = parts[i]
        new = None
        if len(p) >= 3 and p[-2:] == ["==", "0"] and is_atom_seq(p[:-2]):
            new = ["!"] + wrap_atom(p[:-2])
        elif len(p) >= 2 and p[0] == "!" and is_atom_seq(p[1:]):
            new = p[1:] + ["==", "0"]
        elif len(p) >= 3 and p[-2:] == ["!=", "0"] and is_atom_seq(p[:-2]):
            new = p[:-2]
        elif is_atom_seq(p) and not NUM_RE.match(p[0]) and len(p) >= 1:
            new = p + ["!=", "0"]
        if new is not None:
            parts[i] = new
            out = []
            for j, q in enumerate(parts):
                out += q
                if j < len(ops):
                    out.append(ops[j])
            return out
    return None


def wrap_atom(p):
    return p if len(p) == 1 or (p[0] != "(" or match_group(p, 0) != len(p) - 1) and is_simple_chain(p) else ["("] + p + [")"]


def is_simple_chain(p):
    r = operand_right(p, 0)
    return bool(r) and r[1] == len(p)


def is_atom_seq(p):
    if not p:
        return False
    if p[0] in ("!", "-", "~"):
        return False
    r = operand_right(p, 0)
    return bool(r) and r[1] == len(p)


def _const_val(seg):
    """value and formatter for a literal operand ([-]NUM) or None"""
    neg = False
    if len(seg) == 2 and seg[0] == "-":
        neg, seg = True, seg[1:]
    if len(seg) == 1 and NUM_RE.match(seg[0]) and not seg[0].lower().endswith(("u", "l")):
        v = int(seg[0], 0)
        hexf = seg[0].lower().startswith("0x")
        return (-v if neg else v), hexf
    return None


def _fmt_const(v, hexf):
    if v < 0:
        return ["-", (hex(-v) if hexf else str(-v))]
    return [hex(v) if hexf else str(v)]


def rule_cmp_strict(toks, rng):
    sites = binary_sites(toks, {"<", "<=", ">", ">="})
    cands = []
    for L, k, R in sites:
        cl, cr = _const_val(toks[L[0]:L[1]]), _const_val(toks[R[0]:R[1]])
        if cr is not None and cl is None:
            cands.append((L, k, R, "R", cr))
        elif cl is not None and cr is None:
            cands.append((L, k, R, "L", cl))
    if not cands:
        return None
    L, k, R, side, (v, hexf) = rng.choice(cands)
    t = toks[k]
    # normalise to "x op K" (K on the right)
    if side == "L":
        t = CMP_FLIP[t]
    nv, nt = v, t
    if t == "<=":
        nt, nv = "<", v + 1
    elif t == "<":
        nt, nv = "<=", v - 1
    elif t == ">":
        nt, nv = ">=", v + 1
    elif t == ">=":
        nt, nv = ">", v - 1
    x = toks[R[0]:R[1]] if side == "L" else toks[L[0]:L[1]]
    kk = _fmt_const(nv, hexf)
    if side == "L":
        nt = CMP_FLIP[nt]
        return toks[:L[0]] + kk + [nt] + x + toks[R[1]:]
    return toks[:L[0]] + x + [nt] + kk + toks[R[1]:]


EXPR_RULES = [rule_flip_cmp, rule_commute, rule_commute_logic, rule_compound_form, rule_not_eq0, rule_cmp_strict]


def negate_cond(text):
    toks = tokenize(text)
    parts, ops = split_conj(toks)
    if not ops:
        return render(negate_atom(toks))
    # De Morgan
    neg_ops = {"&&": "||", "||": "&&"}
    out = []
    for i, p in enumerate(parts):
        out += negate_atom(p, paren=True)
        if i < len(ops):
            out.append(neg_ops[ops[i]])
    return render(out)


def negate_atom(p, paren=False):
    cmps = [k for k, t in enumerate(p) if t in CMP_NEG and depth_at(p, k) == 0]
    if len(cmps) == 1 and not any(t in ("?", "=") or t in ASSIGN_OPS for t in p):
        k = cmps[0]
        return p[:k] + [CMP_NEG[p[k]]] + p[k + 1:]
    if len(p) >= 2 and p[0] == "!" and is_atom_seq(p[1:]):
        return p[1:]
    if is_atom_seq(p):
        return ["!"] + p
    return ["!", "("] + p + [")"]


def depth_at(p, k):
    d = 0
    for t in p[:k]:
        if t in ("(", "["):
            d += 1
        elif t in (")", "]"):
            d -= 1
    return d


# ---------------- mutations ----------------

def node_text(n):
    out = []
    P.pr_node(n, 0, out)
    return " ".join(out)


def focus_cands(cands, rng, getnodes):
    if FOCUS is not None and rng.random() < 0.7:
        f = [c for c in cands if any(FOCUS.search(node_text(x)) for x in getnodes(c))]
        if f:
            return f
    return cands


def m_swap_adjacent(tree, rng):
    cands = []
    for b in blocks_of(tree):
        for i in range(len(b.items) - 1):
            x, y = b.items[i], b.items[i + 1]
            if x.kind == "label" or y.kind == "label":
                continue
            if not conflict(x, y):
                cands.append((b, i))
    if not cands:
        return False
    cands = focus_cands(cands, rng, lambda c: c[0].items[c[1]:c[1] + 2])
    b, i = rng.choice(cands)
    b.items[i], b.items[i + 1] = b.items[i + 1], b.items[i]
    return True


def m_move_stmt(tree, rng):
    """move one statement several positions if independent of all it crosses"""
    cands = []
    for b in blocks_of(tree):
        n = len(b.items)
        for i in range(n):
            x = b.items[i]
            if x.kind == "label":
                continue
            for j in range(n):
                if j == i or abs(j - i) < 2:
                    continue
                lo, hi = (i + 1, j) if j > i else (j, i - 1)
                if all(b.items[t].kind != "label" and not conflict(x, b.items[t]) for t in range(lo, hi + 1)):
                    cands.append((b, i, j))
    if not cands:
        return False
    b, i, j = rng.choice(cands)
    x = b.items.pop(i)
    b.items.insert(j, x)
    return True


def decl_runs(b):
    return [i for i, it in enumerate(b.items) if is_decl(it)]


def m_decl_shuffle(tree, rng):
    cands = [b for b in blocks_of(tree) if len(decl_runs(b)) >= 2 or any(len(parse_decl(b.items[i].text)[1]) > 1 for i in decl_runs(b))]
    if not cands:
        return False
    b = rng.choice(cands)
    idx = decl_runs(b)
    mode = rng.choice(["perm", "swap", "split", "merge", "inner"])
    if mode == "swap" and len(idx) >= 2:
        i = rng.randrange(len(idx) - 1)
        a, c = idx[i], idx[i + 1]
        if conflict(b.items[a], b.items[c]) and False:
            return False
        b.items[a], b.items[c] = b.items[c], b.items[a]
        return True
    if mode == "perm":
        run = idx
        sub = [b.items[i] for i in run]
        rng.shuffle(sub)
        for i, n in zip(run, sub):
            b.items[i] = n
        return True
    if mode == "split":
        multi = [i for i in idx if len(parse_decl(b.items[i].text)[1]) > 1]
        if not multi:
            return False
        i = rng.choice(multi)
        typ, ds = parse_decl(b.items[i].text)
        news = [make_decl(typ, [d]) for d in ds]
        if rng.random() < 0.5:
            rng.shuffle(news)
        b.items[i:i + 1] = news
        return True
    if mode == "merge":
        pairs = []
        for k in range(len(b.items) - 1):
            x, y = b.items[k], b.items[k + 1]
            if is_decl(x) and is_decl(y):
                tx, dx = parse_decl(x.text)
                ty, dy = parse_decl(y.text)
                if tx == ty:
                    pairs.append(k)
        if not pairs:
            return False
        k = rng.choice(pairs)
        tx, dx = parse_decl(b.items[k].text)
        ty, dy = parse_decl(b.items[k + 1].text)
        b.items[k:k + 2] = [make_decl(tx, dx + dy)]
        return True
    if mode == "inner":
        multi = [i for i in idx if len(parse_decl(b.items[i].text)[1]) > 1]
        if not multi:
            return False
        i = rng.choice(multi)
        typ, ds = parse_decl(b.items[i].text)
        rng.shuffle(ds)
        b.items[i] = make_decl(typ, ds)
        return True
    return False


def parse_single(n):
    d = parse_decl(n.text) if n.kind == "simple" else None
    if d and len(d[1]) == 1:
        return d
    return None


def m_decl_move(tree, rng):
    cands = []
    for b in blocks_of(tree):
        for i, it in enumerate(b.items):
            d = parse_single(it)
            if not d:
                continue
            typ, ds = d
            stars, name, arr, init = ds[0]
            if init is not None and not is_pure_operand(init):
                continue
            uses = [j for j in range(len(b.items)) if j != i and mentions(b.items[j], name)]
            if not uses:
                continue
            first = min(uses)
            if first > i + 1:
                cands.append(("later", b, i, first))
            if i > 0 and first > 0 and init is None:
                cands.append(("top", b, i, 0))
            if first < i:
                pass
            # into inner block when all uses are within one compound child
            if len(uses) == 1 and b.items[uses[0]].kind in ("if", "switch", "for", "while", "do", "block") and uses[0] > i:
                cands.append(("inner", b, i, uses[0]))
    if not cands:
        return False
    kind, b, i, j = rng.choice(cands)
    it = b.items.pop(i)
    if kind == "later":
        b.items.insert(j - 1, it)
        return True
    if kind == "top":
        b.items.insert(0, it)
        return True
    tgt = b.items[j - 1]
    inner = None
    if tgt.kind == "block":
        inner = tgt
    elif tgt.kind == "if":
        # use in then / else: choose the block that mentions
        if mentions(tgt.then, parse_single(it)[1][0][1]) and not (tgt.els and mentions(tgt.els, parse_single(it)[1][0][1])) \
                and not mentions_text(tgt.cond, parse_single(it)[1][0][1]):
            inner = tgt.then
        elif tgt.els and mentions(tgt.els, parse_single(it)[1][0][1]) and not mentions(tgt.then, parse_single(it)[1][0][1]) \
                and not mentions_text(tgt.cond, parse_single(it)[1][0][1]):
            inner = tgt.els
    elif tgt.kind in ("for", "while", "do"):
        nm = parse_single(it)[1][0][1]
        if not mentions_text(tgt.cond, nm) and not (tgt.kind == "for" and (mentions_text(tgt.init, nm) or mentions_text(tgt.inc, nm))):
            first_stmt = tgt.body.items[0] if tgt.body.items else None
            if first_stmt is not None and first_stmt.kind == "simple" and re.match(r"^%s = " % re.escape(nm), first_stmt.text):
                inner = tgt.body
    if inner is None:
        b.items.insert(j - 1, it)
        return False
    inner.items.insert(0, it)
    inner.braced = True
    return True


def mentions_text(text, name):
    inf = text_info(text)
    return _has_name(inf, name)


def m_decl_hoist_outer(tree, rng):
    """move a declaration from an inner block to the top of the function"""
    top = tree
    cands = []
    for b in blocks_of(tree):
        if b is top:
            continue
        for i, it in enumerate(b.items):
            d = parse_single(it)
            if d and d[1][0][3] is None:
                cands.append((b, i))
    if not cands:
        return False
    b, i = rng.choice(cands)
    it = b.items.pop(i)
    nm = parse_single(it)[1][0][1]
    names = set()
    for n in top.items:
        d = parse_decl(n.text) if n.kind == "simple" else None
        if d:
            names |= {x[1] for x in d[1]}
    if nm in names:
        b.items.insert(i, it)
        return False
    pos = len(decl_runs(top)) and rng.choice(decl_runs(top) + [max(decl_runs(top)) + 1]) or 0
    top.items.insert(pos, it)
    return True


def m_decl_init_merge(tree, rng):
    cands = []
    for b in blocks_of(tree):
        for i, it in enumerate(b.items):
            d = parse_single(it)
            if not d:
                continue
            typ, ds = d
            stars, name, arr, init = ds[0]
            if arr:
                continue
            if init is None:
                uses = [j for j in range(i + 1, len(b.items)) if mentions(b.items[j], name)]
                if not uses:
                    continue
                j = uses[0]
                s = b.items[j]
                if s.kind == "simple" and re.match(r"^%s = " % re.escape(name), s.text) and not text_has_name(s.text[len(name) + 3:], name):
                    cands.append(("merge", b, i, j))
            else:
                cands.append(("split", b, i, i))
    if not cands:
        return False
    kind, b, i, j = rng.choice(cands)
    typ, ds = parse_single(b.items[i])
    stars, name, arr, init = ds[0]
    if kind == "merge":
        s = b.items[j]
        rhs = tokenize(s.text[len(name) + 3:])
        b.items.pop(j)
        b.items[i] = make_decl(typ, [(stars, name, arr, rhs)])
        # keep position: declaration moves to j
        it = b.items.pop(i)
        b.items.insert(j - 1, it)
    else:
        b.items[i] = make_decl(typ, [(stars, name, arr, None)])
        b.items.insert(i + 1, Simple(name + " = " + render(init)))
    return True


def text_has_name(text, name):
    return name in set(tokenize(text))


def m_toggle_signed(tree, rng):
    cands = []
    for b in blocks_of(tree):
        for i, it in enumerate(b.items):
            d = parse_decl(it.text) if it.kind == "simple" else None
            if d and d[0] in (["int"], ["unsigned"], ["unsigned", "int"], ["char"], ["unsigned", "char"], ["short"], ["unsigned", "short"]) \
                    and not any(x[0] for x in d[1]) :
                cands.append((b, i, d))
    if not cands:
        return False
    b, i, (typ, ds) = rng.choice(cands)
    if typ[0] == "unsigned":
        base = typ[1:] or ["int"]
        nt = base if base != ["int"] or rng.random() < 0.5 else ["int"]
    else:
        nt = ["unsigned"] + ([typ[0]] if typ[0] != "int" else rng.choice([[], ["int"]]))
    b.items[i] = make_decl(nt, ds)
    return True


def m_decl_type_char_int(tree, rng):
    """int <-> char-sized flips are not semantics preserving; only widen unsigned char <-> int is skipped."""
    return False


FOCUS = None


def focus_slots(slots, rng):
    if FOCUS is not None and rng.random() < 0.7:
        f = [x for x in slots if FOCUS.search(getattr(x[0], x[1]) or "")]
        if f:
            return f
    return slots


def m_expr_rule(tree, rng):
    slots = focus_slots(text_slots(tree), rng)
    rng.shuffle(slots)
    for node, attr in slots[:12]:
        text = getattr(node, attr)
        if not text:
            continue
        toks = tokenize(text)
        d = parse_decl(text) if node.kind == "simple" else None
        if d:
            # apply to initializers only
            continue
        rules = [r for r in EXPR_RULES if r is not rule_not_eq0 or attr == "cond"]
        rng.shuffle(rules)
        for rule in rules:
            new = rule(toks, rng)
            if new is not None and new != toks:
                setattr(node, attr, render(new))
                return True
        if (node.kind == "simple" or (node.kind == "for" and attr == "inc")) and len(toks) == 2:
            new = rule_incdec_form(toks, rng)
            if new:
                setattr(node, attr, render(new))
                return True
    return False


def m_incdec(tree, rng):
    cands = []
    for node, attr in text_slots(tree):
        if node.kind == "simple" or (node.kind == "for" and attr in ("inc", "init")):
            toks = tokenize(getattr(node, attr))
            if len(toks) == 2 and ((toks[0] in ("++", "--") and IDENT_RE.match(toks[1])) or (toks[1] in ("++", "--") and IDENT_RE.match(toks[0]))):
                cands.append((node, attr, toks))
    if not cands:
        return False
    node, attr, toks = rng.choice(cands)
    setattr(node, attr, render([toks[1], toks[0]]))
    return True


def m_if_swap(tree, rng):
    cands = [n for n in all_nodes(tree) if n.kind == "if" and n.els is not None]
    if not cands:
        return False
    n = rng.choice(cands)
    n.cond = negate_cond(n.cond)
    n.then, n.els = n.els, n.then
    if n.then.kind == "block" and not n.then.braced and len(n.then.items) == 1 and n.then.items[0].kind == "if":
        n.then.braced = True
    return True


def m_if_nest(tree, rng):
    cands = []
    for n in all_nodes(tree):
        if n.kind == "if" and n.els is None:
            toks = tokenize(n.cond)
            parts, ops = split_conj(toks)
            if len(parts) >= 2 and all(o == "&&" for o in ops):
                cands.append(("split", n))
            if len(n.then.items) == 1 and n.then.items[0].kind == "if" and n.then.items[0].els is None:
                cands.append(("merge", n))
    if not cands:
        return False
    kind, n = rng.choice(cands)
    if kind == "split":
        parts, ops = split_conj(tokenize(n.cond))
        k = rng.randrange(1, len(parts))
        a = render(join_conj(parts[:k], ops[:k - 1]))
        bcond = render(join_conj(parts[k:], ops[k:]))
        inner = If(bcond, n.then, None)
        n.cond = a
        n.then = Block([inner], braced=False)
    else:
        inner = n.then.items[0]
        c1, c2 = n.cond, inner.cond
        n.cond = wrap_or(c1) + " && " + wrap_or(c2)
        n.then = inner.then
    return True


def wrap_or(c):
    parts, ops = split_conj(tokenize(c))
    if "||" in ops:
        return "(" + c + ")"
    return c


def join_conj(parts, ops):
    out = []
    for i, p in enumerate(parts):
        out += p
        if i < len(ops):
            out.append(ops[i])
    return out


def m_early_return(tree, rng):
    cands = []
    for b in blocks_of(tree):
        for i, it in enumerate(b.items):
            if it.kind == "if" and it.els is None and is_terminal(it.then) and i + 1 < len(b.items):
                if not any(x.kind == "label" for x in b.items[i + 1:]) and not any(is_decl(x) for x in b.items[i + 1:]):
                    cands.append(("wrap", b, i))
            if it.kind == "if" and it.els is not None and is_terminal(it.then):
                cands.append(("unwrap", b, i))
    if not cands:
        return False
    kind, b, i = rng.choice(cands)
    it = b.items[i]
    if kind == "wrap":
        rest = b.items[i + 1:]
        del b.items[i + 1:]
        it.els = Block(rest, braced=True)
    else:
        els = it.els
        rest = els.items if els.kind == "block" else [els]
        it.els = None
        b.items[i + 1:i + 1] = rest
    return True


def m_ternary(tree, rng):
    cands = []
    for b in blocks_of(tree):
        for i, it in enumerate(b.items):
            if it.kind == "if" and it.els is not None and len(it.then.items) == 1 and len(it.els.items) == 1:
                x, y = it.then.items[0], it.els.items[0]
                if x.kind == "simple" and y.kind == "simple":
                    mx = re.match(r"^(.*?[^=!<>+\-*/&|^%])= (.*)$", x.text)
                    my = re.match(r"^(.*?[^=!<>+\-*/&|^%])= (.*)$", y.text)
                    if mx and my and mx.group(1) == my.group(1) and "?" not in x.text + y.text:
                        cands.append(("to", b, i, mx.group(1), mx.group(2), my.group(2)))
                    mr = re.match(r"^return (.*)$", x.text)
                    mr2 = re.match(r"^return (.*)$", y.text)
                    if mr and mr2 and "?" not in x.text + y.text:
                        cands.append(("toret", b, i, None, mr.group(1), mr2.group(1)))
            if it.kind == "simple":
                m = re.match(r"^(.*?[^=!<>+\-*/&|^%])= \(?(.*?)\)? \? ([^:?]*) : ([^:?]*)$", it.text)
                if m:
                    cands.append(("from", b, i, m.group(1), (m.group(2), m.group(3), m.group(4)), None))
                m = re.match(r"^return \(?(.*?)\)? \? ([^:?]*) : ([^:?]*)$", it.text)
                if m:
                    cands.append(("fromret", b, i, None, (m.group(1), m.group(2), m.group(3)), None))
    if not cands:
        return False
    kind, b, i, lv, a, c = rng.choice(cands)
    it = b.items[i]
    if kind == "to":
        b.items[i] = Simple("%s= %s ? %s : %s" % (lv, wrap_cond(it.cond), a, c))
    elif kind == "toret":
        b.items[i] = Simple("return %s ? %s : %s" % (wrap_cond(it.cond), a, c))
    elif kind == "from":
        cond, x, y = a
        b.items[i] = If(cond, Block([Simple("%s= %s" % (lv, x))], braced=False), Block([Simple("%s= %s" % (lv, y))], braced=False))
    else:
        cond, x, y = a
        b.items[i] = If(cond, Block([Simple("return " + x)], braced=False), Block([Simple("return " + y)], braced=False))
    return True


def wrap_cond(c):
    toks = tokenize(c)
    if is_atom_seq(toks):
        return c
    return "(" + c + ")"


def case_groups(sw):
    groups = []
    cur = None
    for it in sw.body.items:
        if it.kind == "label" and (it.text.startswith("case") or it.text.startswith("default")):
            if cur is None or cur["body"]:
                cur = {"labels": [it], "body": []}
                groups.append(cur)
            else:
                cur["labels"].append(it)
        else:
            if cur is None:
                return None
            cur["body"].append(it)
    return groups


def flatten_groups(groups):
    out = []
    for g in groups:
        out += g["labels"] + g["body"]
    return out


def m_switch_reorder(tree, rng):
    sws = [n for n in all_nodes(tree) if n.kind == "switch"]
    if not sws:
        return False
    sw = rng.choice(sws)
    groups = case_groups(sw)
    if not groups or len(groups) < 2:
        return False
    mode = rng.choice(["swap", "labels", "move"])
    if mode == "labels":
        multi = [g for g in groups if len(g["labels"]) > 1]
        if not multi:
            return False
        g = rng.choice(multi)
        rng.shuffle(g["labels"])
        sw.body.items = flatten_groups(groups)
        return True
    n = len(groups)
    if mode == "swap":
        i = rng.randrange(n - 1)
        j = i + 1
    else:
        i, j = rng.sample(range(n), 2)
    new = groups[:]
    g = new.pop(i)
    new.insert(j, g)
    # validity: each non-terminal group must still be followed by its original successor
    succ = {id(groups[k]): (groups[k + 1] if k + 1 < n else None) for k in range(n)}
    for k, g in enumerate(new):
        term = bool(g["body"]) and is_terminal_tail(g["body"])
        nxt = new[k + 1] if k + 1 < len(new) else None
        if not term and succ[id(g)] is not nxt:
            return False
    sw.body.items = flatten_groups(new)
    return True


def is_terminal_tail(items):
    return bool(items) and is_terminal(items[-1])


def m_loop_convert(tree, rng):
    cands = []
    for b in blocks_of(tree):
        for i, it in enumerate(b.items):
            if it.kind == "for" and not has_continue(it.body):
                cands.append(("for2while", b, i))
            if it.kind == "while":
                cands.append(("while2for", b, i))
            if it.kind == "for" and it.init == "" and it.inc == "":
                cands.append(("for2while", b, i))
    if not cands:
        return False
    kind, b, i = rng.choice(cands)
    it = b.items[i]
    if kind == "for2while":
        body_items = list(it.body.items)
        if it.inc:
            body_items.append(Simple(it.inc))
        w = While(it.cond or "1", Block(body_items, braced=True))
        new = ([Simple(it.init)] if it.init else []) + [w]
        b.items[i:i + 1] = new
        return True
    f = For("", it.cond, "", it.body)
    b.items[i] = f
    return True


def has_continue(n):
    for x in all_nodes(n):
        if x.kind == "simple" and tokenize(x.text)[:1] == ["continue"]:
            return True
    return False


def m_loop_cmp(tree, rng):
    cands = []
    for n in all_nodes(tree):
        if n.kind == "for":
            it = tokenize(n.inc)
            ct = tokenize(n.cond)
            if len(it) == 2 and ct:
                v = it[1] if it[0] in ("++", "--") else it[0]
                op = it[0] if it[0] in ("++", "--") else it[1]
                if ct[0] == v and len(ct) >= 3 and ct[1] in ("<", "!=", ">") and not any(t in ("&&", "||") for t in ct):
                    if (op == "++" and ct[1] in ("<", "!=")) or (op == "--" and ct[1] in (">", "!=")):
                        cands.append((n, v, op, ct))
    if not cands:
        return False
    n, v, op, ct = rng.choice(cands)
    if ct[1] == "!=":
        ct[1] = "<" if op == "++" else ">"
    else:
        ct[1] = "!="
    n.cond = render(ct)
    return True


def m_inline_single(tree, rng):
    cands = []
    for b in blocks_of(tree):
        for i, it in enumerate(b.items):
            d = parse_single(it)
            if not d:
                continue
            typ, ds = d
            stars, name, arr, init = ds[0]
            if arr or init is None:
                continue
            uses = [j for j in range(i + 1, len(b.items)) if mentions(b.items[j], name)]
            if len(uses) != 1:
                continue
            j = uses[0]
            others_use = sum(1 for n in all_nodes(b.items[j]) for (nn, at) in [(n, None)] if False)
            tgt = b.items[j]
            cnt = count_mentions(tgt, name)
            if cnt != 1:
                continue
            # between statements must not conflict with the initializer
            probe = Simple(render(init))
            if any(conflict(probe, b.items[t]) for t in range(i + 1, j)):
                continue
            if tgt.kind in ("for", "while", "do"):
                continue
            cands.append((b, i, j, name, init))
    if not cands:
        return False
    b, i, j, name, init = rng.choice(cands)
    tgt = b.items[j]
    rep = init if is_atom_seq(init) else ["("] + init + [")"]
    done = []

    def sub(text):
        toks = tokenize(text)
        out = []
        for t in toks:
            if t == name:
                out += rep
                done.append(1)
            else:
                out.append(t)
        return render(out)

    for node, attr in text_slots(tgt):
        txt = getattr(node, attr)
        if name in tokenize(txt):
            setattr(node, attr, sub(txt))
    for n in all_nodes(tgt):
        pass
    if not done:
        return False
    b.items.pop(i)
    return True


def count_mentions(node, name):
    c = 0
    for n in all_nodes(node):
        for nn, attr in text_slots(n) if n.kind != "block" else []:
            pass
    for node2, attr in text_slots(node):
        if node2 is node or True:
            pass
    seen = set()
    for n, attr in text_slots(node):
        if (id(n), attr) in seen:
            continue
        seen.add((id(n), attr))
        c += sum(1 for t in tokenize(getattr(n, attr)) if t == name)
    return c


MUTATIONS = [
    (m_swap_adjacent, 3), (m_move_stmt, 2), (m_decl_shuffle, 3), (m_decl_move, 2), (m_decl_hoist_outer, 1),
    (m_decl_init_merge, 2), (m_toggle_signed, 1), (m_expr_rule, 5), (m_incdec, 1), (m_if_swap, 2), (m_if_nest, 2),
    (m_early_return, 2), (m_ternary, 1), (m_switch_reorder, 2), (m_loop_convert, 1), (m_loop_cmp, 1),
    (m_inline_single, 2),
]


def mutate(tree, rng, tries=30):
    """returns (new_tree, name) or None"""
    import mutate2
    fns = [f for f, w in MUTATIONS + mutate2.MUTATIONS2 for _ in range(w)]
    for _ in range(tries):
        t = copy.deepcopy(tree)
        f = rng.choice(fns)
        try:
            ok = f(t, rng)
        except Exception:
            ok = False
        if ok:
            return t, f.__name__
    return None
