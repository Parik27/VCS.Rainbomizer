/* This file describes all the patterns used in the game. It is to be used in
 * conjunction with the GameAddress class. Whenever you use GameAddress or any
 * of its helper classes, it will use a "resolved" address that converts an
 * initial address (in VCS Rainbomizer, we use ULES00502 version addresses).
 * This facilitates quick testing as we can use addresses temporarily (they are
 * convenient) and once the hooks are more concrete, we can simply add a pattern
 * for the address here to make it version-agnostic.

 Patterns can be assigned a "resolver":
 - NONE: the pattern will return the address as-is without any changes.
 - BRANCH: the pattern must resolve to an address that is a Jal/J instruction
 and the resolver will return the address of the branch the instruction points
 to.
 - GPADDR: the pattern points to a two byte offset of the required address from
 $gp.
 */

#include "Pattern.hh"
#include <array>

inline static constexpr std::array s_Patterns = {
    Pattern{0x08aec324, "15 3c ? ? b6 26 25 28 c0 03 04 00 06 34 ? ? ? ?", 14},
    Pattern{0x08b3d0a0, "25 ? c0 02 25 20 c0 03 ? ? ? ? 25 28 40 00 25 20 00 00", -4},
    Pattern{0x08906354, "25 80 80 00 ? ? ? ? 50 00 04 8e 50 00 04 8e ? ? ? ? bc 02 05 26 56 00 07 86", -4},
    Pattern{0x08b2a76c, "25 28 00 01 ? ? ? ? 00 00 00 00", -4},
    Pattern{0x08968524, "27 ? ? ? ? 25 20 20 02 ? ? ? ? 00 2a 04 00", -7},
    Pattern{0x08abc41c, "01 00 06 34 ? ? ? ? ? ? 80 18", -4},
    Pattern{0x089de070, "25 ? 60 02 ? ? ? ? ? ? ? 27 ? ? ? ? 10 00 a5 27", -4} ,
    Pattern{0x08935e7c, "24 ? ? ? 27 25 28 00 00 25 30 00 00", -7},
    Pattern{0x0891b7dc, "C0 FF BD 27 ? ? B1 AF ? ? B2 AF 25 88 A0 00 25 90 C0 00", 0},
    Pattern{0x08908080, "F0 FF BD 27 ? ? B1 AF FF FF 06 24", 0},
    Pattern{0x088f5a3c, "D0 FF BD 27 ? ? B0 AF ? ? B1 AF ? ? B2 AF ? ? B3 AF ? ? B4 AF ? ? B5 AF ? ? B6 AF ? ? BF AF FF 00 D2 30", 0},
    Pattern{0x088f47bc, "ff ff 09 24 3a 00 64 82 02 00 84 30", -4},
    Pattern{0x08b4275c, "D0 FF BD 27 ? ? 86 93 ? ? B3 AF", 0},
    Pattern{0x08aed784, "25 20 00 02 28 00 04 8e ? ? 91 10", -4},
    Pattern{0x0896075c, "21 28 84 00 00 32 04 00 21 20 85 00 40 21 04 00 ? ? 05 3c 21 20 c4 00 ? ? a5 24 21 20 85 00 08 00 e0 03", -4},
    Pattern{0x08ad3660, "F0 FF BD 27 25 38 A0 00 25 28 80 00 ? ? 84 8F"},
    Pattern{0x08ad39f0, "F0 FF BD 27 FF 00 86 30"},
    Pattern{0x08a0b5a8, "B0 FE BD 27 ? ? B0 AF ? ? B1 AF ? ? B2 AF ? ? B3 AF", 0},
    Pattern{0x08a0b608, "C6 94 ? ? A6 14 00 00 00 00 ? ? 00 12", -2},
    Pattern{0x08a0b60c, "A6 14 00 00 00 00 ? ? 00 12", -2},
    Pattern{0x089bd2d4, "80 10 00 00 00 00 ? ? ? ? 00 00 00 00 25 80 40 00 ? ? ? ? 25 20 00 02 ? ? 40 10", -2},
    Pattern{0x08a0b62c, "A6 10 00 00 00 00 ? ? 94 90", -2},
    Pattern{0x0894ad94, "80 10 00 00 00 00 ? ? ? ? 25 20 00 02 ? ? 40 10 00 00 00 00 ? ? ? ? 25 20 00 02 ? ? ? ? 25 20 00 02 ? ? 40 1c 00 00 00 00", -2},
    Pattern{0x08911e6c, "00 00 12 34 ? ? ? ? 25 20 00 02 ? ? ? ? 25 20 00 02 ? ? 40 14 00 00 00 00 00 00 12 34 ? ? 40 12", 0},
    Pattern{0x08949b50, "A6 14 FF 00 84 30 01 00 04 34 FF 00 84 30 ? ? 80 14 00 00 00 00 ? ? 04 8E 01 00 84 30", -2},
    Pattern{0x0894b934, "80 10 00 00 00 00 ? ? 04 8E 08 00 84 30 ? ? 80 10 00 00 00 00 25 20 00 02", -2},
    Pattern{0x089511a4, "85 14 00 00 00 00 ? ? ? ? 25 20 60 02 ? ? 40 54", -2},
    Pattern{0x08a456cc, "30 FF BD 27 ? ? B4 E7 ? ? B6 E7 ? ? B0 AF ? ? B1 AF ? ? B2 AF ? ? B3 AF ? ? B4 AF ? ? B5 AF ? ? BF AF 00 00 13 34"},
    Pattern{0x08a45d50, "A3 3C 04 3C 0A D7 84 34 00 70 84 44"},
    Pattern{0x08a52270, "44 8C 05 00 05 34 ? ? 85 10 00 00 00 00 ? ? 00 10", -2},
    Pattern{0x08a43d98, "44 8C 05 00 05 34 ? ? 85 14 00 00 00 00 ? ? 25 8E", -2},
    Pattern{0x08a52764, "44 8C 05 00 05 34 ? ? 85 14 00 00 00 00 ? ? 04 8E", -2},
    Pattern{0x08a8d730, "25 40 40 02 25 10 00 00", -4},
    Pattern{0x08a411b4, "25 30 60 02 ? ? 40 14 01 00 13 34 ? ? 00 10", -4},
    Pattern{0x08b20150, "C0 39 04 00 ? ? 86 93", 0},
    Pattern{0x088809f8, "10 FE BD 27 ? ? B4 E7 ? ? B6 E7 ? ? B8 E7 ? ? BA E7 ? ? BC E7", 0},
};
