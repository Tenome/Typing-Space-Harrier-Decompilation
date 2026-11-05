"""Second set of mutations: folding of increments/assignments into conditions, loop inversion, chained assignment."""
import re

from cparse import Simple, Block, If, For, While, Do, tokenize, render, IDENT_RE
from mutate import (blocks_of, all_nodes, text_slots, parse_decl, is_decl, split_conj, is_atom_seq, negate_cond,
                    text_info, mentions_text, is_terminal, conflict, wrap_cond, has_continue)


def first_ident_pos(toks):
    for k, t in enumerate(toks):
        if IDENT_RE.match(t):
            return k
        if t not in ("(", "!", "-"):
            return None
    return None


def m_fold_incr(tree, rng):
    """v++; if (v ...)  <->  if (++v ...)"""
    cands = []
    for b in blocks_of(tree):
        for i in range(len(b.items) - 1):
            x, y = b.items[i], b.items[i + 1]
            if x.kind == "simple" and y.kind in ("if", "switch"):
                t = tokenize(x.text)
                if len(t) == 2 and IDENT_RE.match(t[0]) and t[1] in ("++", "--") or len(t) == 2 and t[0] in ("++", "--") and IDENT_RE.match(t[1]):
                    v = t[0] if IDENT_RE.match(t[0]) else t[1]
                    op = t[1] if IDENT_RE.match(t[0]) else t[0]
                    c = tokenize(y.cond if y.kind == "if" else y.expr)
                    if c.count(v) == 1 and c.index(v) <= 3 and not any(q in ("&&", "||", "?") for q in c[:c.index(v)]):
                        cands.append(("fold", b, i, v, op))
        for i, y in enumerate(b.items):
            if y.kind == "if":
                c = tokenize(y.cond)
                for k, tkn in enumerate(c):
                    if tkn in ("++", "--") and k + 1 < len(c) and IDENT_RE.match(c[k + 1]) and (k == 0 or c[k - 1] in ("(", "!", "-")):
                        cands.append(("unfold_pre", b, i, c[k + 1], tkn))
                        break
    if not cands:
        return False
    kind, b, i, v, op = rng.choice(cands)
    if kind == "fold":
        y = b.items[i + 1]
        attr = "cond" if y.kind == "if" else "expr"
        c = tokenize(getattr(y, attr))
        k = c.index(v)
        mode = rng.choice(["pre", "post"]) if False else "pre"
        c[k:k + 1] = [op, v]
        setattr(y, attr, render(c))
        b.items.pop(i)
        return True
    y = b.items[i]
    c = tokenize(y.cond)
    k = next(k for k, t in enumerate(c) if t == op and k + 1 < len(c) and c[k + 1] == v)
    c[k:k + 2] = [v]
    y.cond = render(c)
    b.items.insert(i, Simple(v + op))
    return True


def m_fold_assign(tree, rng):
    """v = e; if (v op ...)  <->  if ((v = e) op ...)"""
    cands = []
    for b in blocks_of(tree):
        for i in range(len(b.items) - 1):
            x, y = b.items[i], b.items[i + 1]
            if x.kind == "simple" and y.kind == "if":
                m = re.match(r"^([A-Za-z_]\w*) = (.*)$", x.text)
                if m and "?" not in m.group(2) and not is_decl(x):
                    c = tokenize(y.cond)
                    v = m.group(1)
                    if c and c[0] == v and c.count(v) == 1:
                        cands.append(("fold", b, i, m.group(1), m.group(2)))
                    elif len(c) >= 2 and c[0] == "!" and c[1] == v and c.count(v) == 1:
                        cands.append(("fold", b, i, m.group(1), m.group(2)))
        for i, y in enumerate(b.items):
            if y.kind == "if":
                m = re.match(r"^(!?)\(([A-Za-z_]\w*) = ([^()]*(?:\([^()]*\))?[^()]*)\)(.*)$", y.cond)
                if m and not (m.group(1) and m.group(4).strip()):
                    cands.append(("unfold", b, i, m, None))
    if not cands:
        return False
    kind, b, i, v, e = rng.choice(cands)
    if kind == "fold":
        y = b.items[i + 1]
        c = tokenize(y.cond)
        k = c.index(v)
        c[k:k + 1] = ["("] + tokenize(v + " = " + e) + [")"]
        if c[0] == "(" and len(c) > 1 and False:
            pass
        y.cond = render(c)
        b.items.pop(i)
        return True
    m = v
    y = b.items[i]
    y.cond = m.group(1) + m.group(2) + m.group(4)
    b.items.insert(i, Simple("%s = %s" % (m.group(2), m.group(3))))
    return True


def m_chain_assign(tree, rng):
    cands = []
    for b in blocks_of(tree):
        for i in range(len(b.items) - 1):
            x, y = b.items[i], b.items[i + 1]
            if x.kind == "simple" and y.kind == "simple" and not is_decl(x) and not is_decl(y):
                mx = re.match(r"^([A-Za-z_]\w*) = (.*)$", x.text)
                my = re.match(r"^([A-Za-z_][\w\.\->\[\]]*) = ([A-Za-z_]\w*)$", y.text)
                if mx and my and my.group(2) == mx.group(1) and "?" not in mx.group(2) and not mentions_text(mx.group(2), my.group(1).split("[")[0].split(".")[0].split("-")[0]):
                    cands.append(("merge", b, i, mx, my))
        for i, x in enumerate(b.items):
            if x.kind == "simple":
                m = re.match(r"^([A-Za-z_][\w\.\->\[\]]*) = ([A-Za-z_]\w*) = (.*)$", x.text)
                if m:
                    cands.append(("split", b, i, m, None))
    if not cands:
        return False
    kind, b, i, m1, m2 = rng.choice(cands)
    if kind == "merge":
        b.items[i:i + 2] = [Simple("%s = %s = %s" % (m2.group(1), m1.group(1), m1.group(2)))]
    else:
        b.items[i:i + 1] = [Simple("%s = %s" % (m1.group(2), m1.group(3))), Simple("%s = %s" % (m1.group(1), m1.group(2)))]
    return True


def m_loop_invert(tree, rng):
    """while (c) B  <->  if (c) do B while (c);  and  while (c) B <-> for (;;) { if (!c) break; B }"""
    cands = []
    for b in blocks_of(tree):
        for i, it in enumerate(b.items):
            if it.kind == "while" and not has_continue(it.body):
                cands.append(("w2dw", b, i))
                cands.append(("w2fbreak", b, i))
            if it.kind == "if" and it.els is None and len(it.then.items) == 1 and it.then.items[0].kind == "do" and it.then.items[0].cond == it.cond:
                cands.append(("dw2w", b, i))
            if it.kind == "for" and not it.init and not it.inc and not it.cond and it.body.items and it.body.items[0].kind == "if":
                f = it.body.items[0]
                if f.els is None and len(f.then.items) == 1 and f.then.items[0].kind == "simple" and f.then.items[0].text == "break":
                    cands.append(("fbreak2w", b, i))
            if it.kind == "do":
                cands.append(("dw2for", b, i))
    if not cands:
        return False
    kind, b, i = rng.choice(cands)
    it = b.items[i]
    if kind == "w2dw":
        b.items[i] = If(it.cond, Block([Do(it.body, it.cond)], braced=True), None)
    elif kind == "w2fbreak":
        brk = If(negate_cond(it.cond), Block([Simple("break")], braced=False), None)
        b.items[i] = For("", "", "", Block([brk] + list(it.body.items), braced=True))
    elif kind == "dw2w":
        d = it.then.items[0]
        b.items[i] = While(d.cond, d.body)
    elif kind == "fbreak2w":
        f = it.body.items[0]
        b.items[i] = While(negate_cond(f.cond), Block(list(it.body.items[1:]), braced=True))
    elif kind == "dw2for":
        brk = If(negate_cond(it.cond), Block([Simple("break")], braced=False), None)
        if has_continue(it.body):
            return False
        b.items[i] = For("", "", "", Block(list(it.body.items) + [brk], braced=True))
    return True


def m_stmt_swap_cond_order(tree, rng):
    """if (a) X; if (b) Y; with exclusive/independent -> swapped (covered by swap_adjacent); here: swap the two arms of &&-chained ifs"""
    return False


MUTATIONS2 = [(m_fold_incr, 2), (m_fold_assign, 2), (m_chain_assign, 1), (m_loop_invert, 2)]


_types = {}


def lookup_type(name):
    if not _types:
        import os
        here = os.path.dirname(os.path.abspath(__file__))
        text = open(os.path.join(here, "..", "..", "include", "game.h"), encoding="latin-1").read()
        for ln in text.split("\n"):
            m = re.match(r"^(?:extern\s+)?\s*([A-Za-z_][\w\s\*]*?)\s*\*?\s*\b((?:m|DAT)_[0-9a-f]+)\b\s*(\[[^\]]*\])?\s*;", ln.strip())
            if m and "(" not in ln:
                stars = "*" if re.search(r"\*\s*" + m.group(2) + r"\b", ln) else ""
                if m.group(3):
                    continue
                _types[m.group(2)] = (m.group(1).strip() + " " + stars).strip() if stars else m.group(1).strip()
    return _types.get(name)


def _assigned_names(tree):
    names = set()
    for node, attr in text_slots(tree):
        toks = tokenize(getattr(node, attr))
        for k, t in enumerate(toks):
            if (t.startswith("m_") or t.startswith("DAT_")):
                nxt = toks[k + 1] if k + 1 < len(toks) else None
                prv = toks[k - 1] if k else None
                from mutate import ASSIGN_OPS
                if nxt in ASSIGN_OPS or nxt in ("++", "--") or prv in ("++", "--", "&") or (nxt == "[" and False):
                    names.add(t)
    return names


def m_cache_member(tree, rng):
    counts = {}
    for node, attr in text_slots(tree):
        if node.kind == "simple" and is_decl(node):
            pass
        for t in tokenize(getattr(node, attr)):
            if t.startswith("m_") or t.startswith("DAT_"):
                counts[t] = counts.get(t, 0) + 1
    assigned = _assigned_names(tree)
    cands = [n for n, c in counts.items() if c >= 2 and n not in assigned and lookup_type(n)]
    if not cands:
        return False
    name = rng.choice(cands)
    typ = lookup_type(name)
    local = "c" + name[name.index("_") + 1:] if False else "cache_" + name
    top = tree
    first = next((i for i, it in enumerate(top.items) if any(name in tokenize(getattr(n, a)) for n, a in text_slots(it))), None)
    if first is None:
        return False
    for n, a in text_slots(top):
        toks = tokenize(getattr(n, a))
        if name in toks:
            setattr(n, a, render([local if t == name else t for t in toks]))
    pos = rng.choice([first, len([1 for it in top.items[:first] if is_decl(it)]) if False else 0, first])
    top.items.insert(pos, Simple("%s %s = %s" % (typ if not typ.endswith("*") else typ, local, name) if not typ.endswith("*") else "%s%s = %s" % (typ, local, name)))
    return True


def m_uncache_local(tree, rng):
    cands = []
    for b in blocks_of(tree):
        for i, it in enumerate(b.items):
            if it.kind == "simple":
                d = parse_decl(it.text)
                if d and len(d[1]) == 1 and d[1][0][3] is not None and len(d[1][0][3]) == 1 and (d[1][0][3][0].startswith("m_") or d[1][0][3][0].startswith("DAT_")):
                    cands.append((b, i, d[1][0][1], d[1][0][3][0]))
    if not cands:
        return False
    b, i, local, name = rng.choice(cands)
    if local in _assigned_locals(tree, local):
        return False
    for n, a in text_slots(tree):
        toks = tokenize(getattr(n, a))
        if n is b.items[i]:
            continue
        if local in toks:
            setattr(n, a, render([name if t == local else t for t in toks]))
    b.items.pop(i)
    return True


def _assigned_locals(tree, local):
    from mutate import ASSIGN_OPS
    out = set()
    for node, attr in text_slots(tree):
        toks = tokenize(getattr(node, attr))
        for k, t in enumerate(toks):
            if t == local:
                nxt = toks[k + 1] if k + 1 < len(toks) else None
                prv = toks[k - 1] if k else None
                if (nxt in ASSIGN_OPS or nxt in ("++", "--") or prv in ("++", "--", "&")) and not (node.kind == "simple" and is_decl(node)):
                    out.add(local)
    return out


MUTATIONS2 += [(m_cache_member, 1), (m_uncache_local, 1)]


def m_ternary_swap(tree, rng):
    cands = []
    for n, a in text_slots(tree):
        if n.kind != "simple":
            continue
        m = re.match(r"^((?:[^?]*?= |return ))\(?([^?:]+?)\)? \? ([^?:]+?) : ([^?:]+)$", n.text)
        if m and not is_decl(n):
            cands.append((n, m))
    if not cands:
        return False
    n, m = rng.choice(cands)
    n.text = "%s%s ? %s : %s" % (m.group(1), wrap_cond(negate_cond(m.group(2))), m.group(4), m.group(3))
    return True


def m_case_merge(tree, rng):
    from mutate import case_groups, flatten_groups, node_text
    sws = [n for n in all_nodes(tree) if n.kind == "switch"]
    if not sws:
        return False
    sw = rng.choice(sws)
    groups = case_groups(sw)
    if not groups:
        return False
    cands = []
    for k in range(len(groups) - 1):
        a, b = groups[k], groups[k + 1]
        if a["body"] and b["body"] and "\n".join(node_text(x) for x in a["body"]) == "\n".join(node_text(x) for x in b["body"]):
            cands.append(("merge", k))
    for k, g in enumerate(groups):
        if len(g["labels"]) > 1 and g["body"]:
            cands.append(("split", k))
    if not cands:
        return False
    kind, k = rng.choice(cands)
    import copy
    if kind == "merge":
        groups[k]["labels"] += groups[k + 1]["labels"]
        del groups[k + 1]
    else:
        g = groups[k]
        if rng.random() < 0.5:
            first, rest = g["labels"][:1], g["labels"][1:]
        else:
            first, rest = g["labels"][:-1], g["labels"][-1:]
        g["labels"] = first
        groups.insert(k + 1, {"labels": rest, "body": copy.deepcopy(g["body"])})
    sw.body.items = flatten_groups(groups)
    return True


def _has_free_break(node):
    if node.kind == "simple":
        return tokenize(node.text)[:1] == ["break"]
    if node.kind == "block":
        return any(_has_free_break(x) for x in node.items)
    if node.kind == "if":
        return _has_free_break(node.then) or (node.els is not None and _has_free_break(node.els))
    return False


def m_switch_if(tree, rng):
    cands = []
    for b in blocks_of(tree):
        for i, it in enumerate(b.items):
            if it.kind == "switch" and is_atom_seq(tokenize(it.expr)):
                groups = case_groups(it)
                if groups and all(len(g["labels"]) == 1 for g in groups) and all(
                        g["body"] and g["body"][-1].kind == "simple" and g["body"][-1].text == "break" for g in groups[:-1] if True):
                    ok = True
                    for g in groups:
                        body = g["body"][:-1] if g["body"] and g["body"][-1].kind == "simple" and g["body"][-1].text == "break" else g["body"]
                        if any(_has_free_break(x) for x in body):
                            ok = False
                    lab = [g["labels"][0].text for g in groups]
                    if ok and sum(1 for t in lab if t.startswith("default")) <= 1 and (not any(t.startswith("default") for t in lab) or lab[-1].startswith("default")):
                        cands.append(("sw2if", b, i))
            if it.kind == "if":
                chain, cur = [], it
                v = None
                good = True
                while cur is not None and cur.kind == "if":
                    c = tokenize(cur.cond)
                    if len(c) == 3 and c[1] == "==" and IDENT_RE.match(c[0]) and (v in (None, c[0])) and re.match(r"^(0x[0-9a-fA-F]+|\d+)$", c[2]):
                        v = c[0]
                        chain.append(cur)
                        els = cur.els
                        cur = els.items[0] if els is not None and len(els.items) == 1 and els.items[0].kind == "if" and not els.braced else None
                        if cur is None:
                            break
                    else:
                        good = False
                        break
                if good and len(chain) >= 2:
                    cands.append(("if2sw", b, i))
    if not cands:
        return False
    kind, b, i = rng.choice(cands)
    it = b.items[i]
    if kind == "sw2if":
        groups = case_groups(it)
        node = None
        dflt = None
        pairs = []
        for g in groups:
            body = list(g["body"])
            if body and body[-1].kind == "simple" and body[-1].text == "break":
                body.pop()
            lab = g["labels"][0].text
            if lab.startswith("default"):
                dflt = Block(body, braced=True)
            else:
                pairs.append(("%s == %s" % (it.expr, lab[5:-1].strip()), Block(body, braced=True)))
        node = dflt
        for cond, body in reversed(pairs):
            node = If(cond, body, node)
        b.items[i] = node
    else:
        items = []
        cur = it
        v = tokenize(it.cond)[0]
        dflt = None
        while True:
            c = tokenize(cur.cond)
            items.append(Label("case %s:" % c[2]))
            items += list(cur.then.items) + [Simple("break")]
            els = cur.els
            if els is None:
                break
            if len(els.items) == 1 and els.items[0].kind == "if" and not els.braced:
                nxt = tokenize(els.items[0].cond)
                if len(nxt) == 3 and nxt[1] == "==" and nxt[0] == v:
                    cur = els.items[0]
                    continue
            items.append(Label("default:"))
            items += list(els.items) + [Simple("break")]
            break
        b.items[i] = Switch(v, Block(items, braced=True))
    return True


MUTATIONS2 += [(m_ternary_swap, 1), (m_case_merge, 2), (m_switch_if, 1)]


def m_hoist_expr(tree, rng):
    from mutate import binary_sites, node_info
    found = {}
    for n, a in text_slots(tree):
        toks = tokenize(getattr(n, a))
        if n.kind == "simple" and is_decl(n):
            pass
        for L, k, R in binary_sites(toks, {"+", "-", "*", "&", "|"}):
            seg = toks[L[0]:R[1]]
            if len(seg) < 3 or any(t in ("++", "--", "=") for t in seg):
                continue
            if any(IDENT_RE.match(seg[i]) and i + 1 < len(seg) and seg[i + 1] == "(" for i in range(len(seg))):
                continue
            found.setdefault(render(seg), []).append((n, a))
    cands = [e for e, v in found.items() if len(v) >= 2]
    if not cands:
        return False
    e = rng.choice(cands)
    etoks = tokenize(e)
    names = {t for t in etoks if IDENT_RE.match(t)}
    top = tree
    first = None
    for i, it in enumerate(top.items):
        slots = text_slots(it)
        if any(e in render(tokenize(getattr(n, a))) for n, a in slots):
            first = i
            break
    if first is None:
        return False
    item = top.items[first]
    inf = node_info(item)
    if names & inf.writes:
        return False
    tname = "t%d" % rng.randrange(1, 9)
    for n, a in text_slots(top):
        toks = tokenize(getattr(n, a))
        sites = [(L[0], R[1]) for L, k, R in binary_sites(toks, {"+", "-", "*", "&", "|"}) if toks[L[0]:R[1]] == etoks]
        for s0, s1 in sorted(sites, reverse=True):
            toks[s0:s1] = [tname]
        if sites:
            setattr(n, a, render(toks))
    top.items.insert(first, Simple("int %s = %s" % (tname, e)))
    return True


MUTATIONS2 += [(m_hoist_expr, 1)]


PARAMS = []  # [(type_text, name)] of the target function, set by perm.py


def parse_params(header):
    m = re.search(r"\((.*)\)\s*$", header)
    if not m:
        return []
    out = []
    for part in m.group(1).split(","):
        part = part.strip()
        if not part or part == "void":
            continue
        mm = re.match(r"^(.*?)([A-Za-z_]\w*)$", part)
        if mm:
            out.append((mm.group(1).strip(), mm.group(2)))
    return out


def m_param_copy(tree, rng):
    from mutate import ASSIGN_OPS
    cands = []
    for typ, name in PARAMS:
        assigned = False
        for n, a in text_slots(tree):
            toks = tokenize(getattr(n, a))
            for k, t in enumerate(toks):
                if t == name:
                    nxt = toks[k + 1] if k + 1 < len(toks) else None
                    prv = toks[k - 1] if k else None
                    if nxt in ASSIGN_OPS or nxt in ("++", "--") or prv in ("++", "--", "&"):
                        assigned = True
        used = any(name in tokenize(getattr(n, a)) for n, a in text_slots(tree))
        if used and assigned:
            cands.append((typ, name))
    if not cands:
        return False
    typ, name = rng.choice(cands)
    local = rng.choice(["p", "cur", "ptr"]) if typ.rstrip().endswith("*") else rng.choice(["v", "n", "cur"])
    taken = {t for n, a in text_slots(tree) for t in tokenize(getattr(n, a))}
    if local in taken:
        local = local + "2"
    for n, a in text_slots(tree):
        toks = tokenize(getattr(n, a))
        if name in toks:
            setattr(n, a, render([local if t == name else t for t in toks]))
    tree.items.insert(0, Simple("%s%s%s = %s" % (typ, "" if typ.endswith("*") else " ", local, name)))
    return True


def m_param_uncopy(tree, rng):
    names = {n for _, n in PARAMS}
    cands = []
    for i, it in enumerate(tree.items):
        d = parse_single_decl(it)
        if d and d[3] is not None and len(d[3]) == 1 and d[3][0] in names:
            cands.append((i, d))
    if not cands:
        return False
    i, d = rng.choice(cands)
    local, pname = d[1], d[3][0]
    for n, a in text_slots(tree):
        if n is tree.items[i]:
            continue
        toks = tokenize(getattr(n, a))
        if local in toks:
            setattr(n, a, render([pname if t == local else t for t in toks]))
    tree.items.pop(i)
    return True


def parse_single_decl(it):
    if it.kind != "simple":
        return None
    d = parse_decl(it.text)
    if d and len(d[1]) == 1:
        return d[1][0]
    return None


MUTATIONS2 += [(m_param_copy, 2), (m_param_uncopy, 1)]


def _is_infinite(n):
    return (n.kind == "for" and n.cond == "" and n.init == "") or (n.kind == "while" and n.cond in ("1", "true"))


def m_ret_to_break(tree, rng):
    """for(;;) { P; if (c) A else return X; }  <->  while (1) { P; if (!c) break; A } return X;"""
    cands = []
    last = tree.items[-1] if tree.items else None
    if last is not None and (_is_infinite(last) or (last.kind == "for" and last.cond == "")):
        body = last.body.items
        if body and body[-1].kind == "if" and body[-1].els is not None and len(body[-1].els.items) == 1 \
                and body[-1].els.items[0].kind == "simple" and body[-1].els.items[0].text.startswith("return"):
            cands.append("to_break")
    for i, it in enumerate(tree.items):
        if i + 2 == len(tree.items) and it.kind in ("for", "while") and (it.kind == "while" and it.cond in ("1", "true") or it.kind == "for" and it.cond == ""):
            nxt = tree.items[-1]
            if nxt.kind == "simple" and nxt.text.startswith("return") and not has_continue(it.body):
                if any(x.kind == "if" and x.els is None and len(x.then.items) == 1 and x.then.items[0].kind == "simple"
                       and x.then.items[0].text == "break" for x in it.body.items):
                    cands.append("to_else")
    if not cands:
        return False
    kind = rng.choice(cands)
    if kind == "to_break":
        loop = tree.items[-1]
        iff = loop.body.items[-1]
        ret = iff.els.items[0]
        neg = If(negate_cond(iff.cond), Block([Simple("break")], braced=False), None)
        loop.body.items[-1:] = [neg] + list(iff.then.items)
        loop.body.braced = True
        tree.items.append(Simple(ret.text))
        return True
    loop = tree.items[-2]
    ret = tree.items[-1]
    body = loop.body.items
    k = next(j for j, x in enumerate(body) if x.kind == "if" and x.els is None and len(x.then.items) == 1
             and x.then.items[0].kind == "simple" and x.then.items[0].text == "break")
    iff = body[k]
    rest = body[k + 1:]
    if not rest:
        return False
    new_if = If(negate_cond(iff.cond), Block(list(rest), braced=True), Block([Simple(ret.text)], braced=False))
    loop.body.items = body[:k] + [new_if]
    loop.body.braced = True
    tree.items.pop()
    return True


MUTATIONS2 += [(m_ret_to_break, 2)]


def m_shuffle_run(tree, rng):
    """random permutation of a run of 3-6 pairwise independent consecutive statements"""
    cands = []
    for b in blocks_of(tree):
        n = len(b.items)
        for i in range(n):
            for ln in (3, 4, 5, 6):
                if i + ln > n:
                    break
                run = b.items[i:i + ln]
                if any(x.kind == "label" for x in run):
                    break
                if all(not conflict(run[x], run[y]) for x in range(ln) for y in range(x + 1, ln)):
                    cands.append((b, i, ln))
    if not cands:
        return False
    b, i, ln = rng.choice(cands)
    run = b.items[i:i + ln]
    new = run[:]
    for _ in range(5):
        rng.shuffle(new)
        if new != run:
            break
    b.items[i:i + ln] = new
    return new != run


MUTATIONS2 += [(m_shuffle_run, 2)]
