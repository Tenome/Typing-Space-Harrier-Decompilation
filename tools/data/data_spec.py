# Segment definitions for data_gen_K.py (executed inside its namespace).

def render_romaji(seg):
    rows = []
    for r in range((seg.end - seg.addr) // 36):
        a = seg.addr + r * 36
        kana = c_literal(cstring(a))
        variants = ", ".join(c_literal(cstring(a + 7 + 7 * k)) for k in range(4))
        flags = im.read(a + 35, 1)[0]
        rows.append("    { %s, { %s }, %d }%s" % (kana, variants, flags, decoded_comment(cstring(a))))
    return "RomajiEntry DAT_00429040[0x19e] = {\n%s\n};\n" % ",\n".join(rows)


romaji = add(0x429040, 0x429040 + 36 * 0x19e, "custom", "RomajiEntry", fields=render_romaji)

cos_table = add(0x42ca9c, 0x42ca9c + 720 * 4, "ints", "int")
sin_table = add(0x42d5dc, 0x42d5dc + 720 * 4, "ints", "int")

FILES = [
    ("K_romaji.cpp", "", [romaji]),
    ("K_trig.cpp", "", [cos_table, sin_table]),
]

# ---- 0x42e11c-0x430840: resource name tables and the sound-effect table
res_dir = add(0x42e11c, 0x42e120, "ptrs", "char *")
res_dir2 = add(0x42e120, 0x42e124, "ptrs", "char *")
res_ext_bmp = add(0x42e124, 0x42e128, "ptrs", "char *")
res_ext_hjt = add(0x42e128, 0x42e12c, "ptrs", "char *")
group_names = add(0x42e12c, 0x42e32c, "ptrs", "char *")
group_counts = add(0x42e32c, 0x42e52c, "ints", "int")
bmp_names = add(0x42e52c, 0x430658, "ptrs", "char *")
no_index = add(0x430658, 0x43065c, "ints", "int")
char_names = add(0x43065c, 0x4306c0, "ptrs", "char *")
boss_names = add(0x4306c0, 0x4306e8, "ptrs", "char *")
se_table = add(0x4306e8, 0x430840, "struct", "SeEntry", fields="pi")

FILES.append(("K_names.cpp", "", [res_dir, res_dir2, res_ext_bmp, res_ext_hjt, group_names, group_counts, bmp_names,
                                  no_index, char_names, boss_names, se_table]))

# ---- 0x430840-0x4bd548: stage timeline script (op + fixed number of int arguments, interpreted by FUN_004078f0)
SCRIPT_OPS = {
    -100000: ("STAGE_OP_RESET", 0), -99999: ("STAGE_OP_GROUP", 1), -99998: ("STAGE_OP_ADD", 8),
    -99997: ("STAGE_OP_END", 0), -99996: ("STAGE_OP_EVENT0", 4), -99995: ("STAGE_OP_EVENT1", 4),
    -99994: ("STAGE_OP_EVENT2", 4), -99993: ("STAGE_OP_EVENT3", 4), -99992: ("STAGE_OP_EVENT4", 4),
    -99991: ("STAGE_OP_EVENT5", 4), -99990: ("STAGE_OP_EVENT6", 4),
}


def render_script(seg):
    lines = []
    a = seg.addr
    while a < seg.end:
        v = s32(a)
        name, nargs = SCRIPT_OPS[v]
        args = [str(s32(a + 4 + 4 * i)) for i in range(nargs)]
        lines.append("    " + ", ".join([name] + args) + ",")
        a += 4 + 4 * nargs
    assert a == seg.end, hex(a)
    lines.append("    -1")
    enum = "enum StageOp {\n%s\n};\n" % ",\n".join("    %s = %d" % (n, v) for v, (n, _) in sorted(SCRIPT_OPS.items()))
    return "%s\nint DAT_00430840[] = {\n%s\n};\n" % (enum, "\n".join(lines))


stage_script = add(0x430840, 0x4bd544 + 4, "custom", "int", fields=render_script)
FILES.append(("K_script.cpp", "", [stage_script]))

# ---- 0x4bd54c-0x4bddf8: menu/option tables (pointer and integer runs split automatically at declared objects)
menu_tables = auto_segments(0x4bd54c, 0x4bd924)
shorts_a = add(0x4bd924, 0x4bdab8, "ints", "short")
shorts_b = add(0x4bdab8, 0x4bdcac, "ints", "short")
menu_tables2 = auto_segments(0x4bdcac, 0x4bdd70)
bgm_files = add(0x4bdd70, 0x4bddf8, "struct", "BgmFile", fields="ip")
stage_rows = add(0x4bddf8, 0x4e0ef8, "struct", "StageRow", fields="i" * 22)
rec14 = add(0x4e0ef8, 0x4e1d94, "struct", "Rec14", fields="i" * 5)
menu_tables3 = auto_segments(0x4e1d94, 0x4e22e0)
wave_rows = add(0x4e22e0, 0x4e2310, "struct", "WaveRow", fields="iii")
menu_tables3 += [wave_rows] + auto_segments(0x4e2310, 0x4e2970)
menu_tables3.append(add(0x4e2970, 0x4e2978, "ints", "char"))

FILES.append(("K_menus.cpp", "", menu_tables + [shorts_a, shorts_b] + menu_tables2 + [bgm_files]))
FILES.append(("K_stagerows.cpp", "", [stage_rows]))
FILES.append(("K_tables.cpp", "", [rec14] + menu_tables3))

# ---- 0x4ec3c8-0x4ecb28: on-screen keyboard and tutorial tables (lane E/J types)
def render_keydefs(seg):
    rows = []
    for r in range((seg.end - seg.addr) // 20):
        a = seg.addr + r * 20
        key = im.read(a + 8, 1)[0]
        name = cstring(a + 9)
        key_text = "'%s'" % chr(key) if 0x20 < key < 0x7F and chr(key) not in "'\\" else str(key)
        rows.append("    { %d, %d, %s, %s, %d, %d }%s" % (s32(a), s32(a + 4), key_text, c_literal(name), s32(a + 12),
                                                         s32(a + 16), decoded_comment(name)))
    return "KeyDef DAT_004ec3d8[48] = {\n%s\n};\n" % ",\n".join(rows)


key_defs = add(0x4ec3d8, 0x4ec798, "custom", "KeyDef", fields=render_keydefs)
tut_a = add(0x4ec3c8, 0x4ec3cc, "ints", "int")
tut_b = add(0x4ec3cc, 0x4ec3d8, "ptrs", "const char *")
tut_c = auto_segments(0x4ec798, 0x4ec7bc)
icon_pos = add(0x4ec7bc, 0x4ec81c, "struct", "IconPos", fields="ii")
tut_d = auto_segments(0x4ec81c, 0x4ec888)
name_timing = add(0x4ec888, 0x4ec978, "struct", "NameTiming", fields="iiii")
label_defs = add(0x4ec978, 0x4ecaf8, "struct", "LabelDef", fields="pi")
tut_e = auto_segments(0x4ecaf8, 0x4ecb28)
FILES.append(("K_keyboard.cpp", "", [tut_a, tut_b, key_defs] + tut_c + [icon_pos] + tut_d + [name_timing, label_defs] + tut_e))
for seg in tut_c:
    if seg.ctype == "const unsigned char *":
        park(seg, "lane E declares DAT_%08x as const unsigned char *; string literals need const char *" % seg.addr,
             "const char *")
park(tut_b, "lane E declares DAT_004ec3cc[0x31]; the table has 3 entries (KeyDef table starts at 0x4ec3d8)")

# ---- 0x4ecfa0: window title pointer and the second kana table (compound kana, same record layout as the first)
def render_romaji_table(seg):
    rows = []
    for r in range((seg.end - seg.addr) // 36):
        a = seg.addr + r * 36
        d = im.read(a, 36)
        if not any(d):
            rows.append("    { \"\", { \"\", \"\", \"\", \"\" }, 0 }")
            continue
        variants = ", ".join(c_literal(cstring(a + 7 + 7 * k)) for k in range(4))
        rows.append("    { %s, { %s }, %d }%s" % (c_literal(cstring(a)), variants, d[35], decoded_comment(cstring(a))))
    return "RomajiEntry %s[%d] = {\n%s\n};\n" % (seg.name, (seg.end - seg.addr) // 36, ",\n".join(rows))


title_ptr = add(0x4ecfa0, 0x4ecfa4, "ptrs", "char *")
title_pad = add(0x4ecfa4, 0x4ecfa8, "ints", "int")
romaji2 = add(0x4ecfa8, 0x4ecfa8 + 36 * 208, "custom", "RomajiEntry", fields=render_romaji_table)
FILES.append(("K_romaji2.cpp", "", [title_ptr, title_pad, romaji2]))

# ---- objects embedded in the string-literal pool (0x4ebb14-0x4ec3bc): Shift-JIS range tables, key shape tables
def render_rgb(seg):
    rows = []
    for r in range((seg.end - seg.addr) // 3):
        a = seg.addr + r * 3
        rows.append("    { 0x%02x, 0x%02x, 0x%02x }" % tuple(im.read(a, 3)))
    return "Rgb DAT_004ec160[144] = {\n%s\n};\n" % ",\n".join(rows)


pool_objs = [
    add(0x4ebb14, 0x4ebb18, "ints", "int"), add(0x4ebb18, 0x4ebb1c, "ints", "int"),
    add(0x4ebb1c, 0x4ebb20, "ints", "int"),
    add(0x4ebb2c, 0x4ebb30, "ints", "unsigned char"),
    add(0x4ebb30, 0x4ebb34, "ints", "int"),
    add(0x4ebb34, 0x4ebbc4, "ints", "unsigned short"),
    add(0x4ebbc4, 0x4ebbc8, "ints", "int"),
    add(0x4ebd60, 0x4ebd64, "ptrs", "char *"),
    add(0x4ebd68, 0x4ec160, "ints", "int"),
    add(0x4ec160, 0x4ec310, "custom", "Rgb", fields=render_rgb),
    add(0x4ec310, 0x4ec3bc, "ints", "int"),
]
FILES.append(("K_pool.cpp", "", pool_objs))
