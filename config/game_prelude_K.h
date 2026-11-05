/* Lane K: types and externs for the initialised data defined in src/K_*.cpp that no other lane declares. */
struct Rgb {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};
extern Rgb DAT_004ec160[144];  /* colour ramps of the key outlines, 3..24 entries each, addressed by FUN_004189d0 */
