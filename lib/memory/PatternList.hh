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
 - GPOFFSET: the pattern points to a two byte offset of the required address from
 $gp.
 */

#pragma once

#include "Pattern.hh"
#include <array>

inline static constexpr std::array s_Patterns = {
    Pattern{0x08bb0578, "80 50 56 00 e4 86 25 20 e0 02 ? ? ? ? 25 28 00 00 ? ? ? ? ff 00 05 30", -6, Pattern::GPOFFSET},
    Pattern{0x08baaf44, "94 8F ? ? 00 10 00 00 14 34", -2, Pattern::GPOFFSET},
    Pattern{0x08b4275c, "D0 FF BD 27 ? ? 86 93 ? ? B3 AF", 0},
    Pattern{0x08b3d0a0, "25 ? c0 02 25 20 c0 03 ? ? ? ? 25 28 40 00 25 20 00 00", -4},
    Pattern{0x08b2a76c, "25 28 00 01 ? ? ? ? 00 00 00 00", -4},
    Pattern{0x08b20150, "C0 39 04 00 ? ? 86 93", 0},
    Pattern{0x08aed784, "25 20 00 02 28 00 04 8e ? ? 91 10", -4},
    Pattern{0x08aec324, "15 3c ? ? b6 26 25 28 c0 03 04 00 06 34 ? ? ? ?", 14},
    Pattern{0x08ad39f0, "F0 FF BD 27 FF 00 86 30"},
    Pattern{0x08ad3660, "F0 FF BD 27 25 38 A0 00 25 28 80 00 ? ? 84 8F"},
    Pattern{0x08ad2b44, "D0 FD BD 27 ? ? B4 E7 ? ? B6 E7 ? ? B8 E7 ? ? BA E7 ? ? BC E7 ? ? B0 AF", 0},
    Pattern{0x08abc41c, "01 00 06 34 ? ? ? ? ? ? 80 18", -4},
    Pattern{0x08a8d730, "25 40 40 02 25 10 00 00", -4},
    Pattern{0x08a52764, "44 8C 05 00 05 34 ? ? 85 14 00 00 00 00 ? ? 04 8E", -2},
    Pattern{0x08a52270, "44 8C 05 00 05 34 ? ? 85 10 00 00 00 00 ? ? 00 10", -2},
    Pattern{0x08a45d50, "A3 3C 04 3C 0A D7 84 34 00 70 84 44"},
    Pattern{0x08a456cc, "30 FF BD 27 ? ? B4 E7 ? ? B6 E7 ? ? B0 AF ? ? B1 AF ? ? B2 AF ? ? B3 AF ? ? B4 AF ? ? B5 AF ? ? BF AF 00 00 13 34"},
    Pattern{0x08a43d98, "44 8C 05 00 05 34 ? ? 85 14 00 00 00 00 ? ? 25 8E", -2},
    Pattern{0x08a411b4, "25 30 60 02 ? ? 40 14 01 00 13 34 ? ? 00 10", -4},
    Pattern{0x08a0b62c, "A6 10 00 00 00 00 ? ? 94 90", -2},
    Pattern{0x08a0b60c, "A6 14 00 00 00 00 ? ? 00 12", -2},
    Pattern{0x08a0b608, "C6 94 ? ? A6 14 00 00 00 00 ? ? 00 12", -2},
    Pattern{0x08a0b5a8, "B0 FE BD 27 ? ? B0 AF ? ? B1 AF ? ? B2 AF ? ? B3 AF", 0},
    Pattern{0x089de070, "25 ? 60 02 ? ? ? ? ? ? ? 27 ? ? ? ? 10 00 a5 27", -4} ,
    Pattern{0x089bd2d4, "80 10 00 00 00 00 ? ? ? ? 00 00 00 00 25 80 40 00 ? ? ? ? 25 20 00 02 ? ? 40 10", -2},
    Pattern{0x08968524, "27 ? ? ? ? 25 20 20 02 ? ? ? ? 00 2a 04 00", -7},
    Pattern{0x0896075c, "21 28 84 00 00 32 04 00 21 20 85 00 40 21 04 00 ? ? 05 3c 21 20 c4 00 ? ? a5 24 21 20 85 00 08 00 e0 03", -4},
    Pattern{0x089511a4, "85 14 00 00 00 00 ? ? ? ? 25 20 60 02 ? ? 40 54", -2},
    Pattern{0x0894b934, "80 10 00 00 00 00 ? ? 04 8E 08 00 84 30 ? ? 80 10 00 00 00 00 25 20 00 02", -2},
    Pattern{0x0894ad94, "80 10 00 00 00 00 ? ? ? ? 25 20 00 02 ? ? 40 10 00 00 00 00 ? ? ? ? 25 20 00 02 ? ? ? ? 25 20 00 02 ? ? 40 1c 00 00 00 00", -2},
    Pattern{0x08949b50, "A6 14 FF 00 84 30 01 00 04 34 FF 00 84 30 ? ? 80 14 00 00 00 00 ? ? 04 8E 01 00 84 30", -2},
    Pattern{0x08947b90, "AA 3F 04 3C 71 3D 84 34"},
    Pattern{0x08947b38, "C0 10 00 00 00 00 ? ? 86 8C ? ? C0 10 00 00 00 00", -2},
    Pattern{0x08935e7c, "24 ? ? ? 27 25 28 00 00 25 30 00 00", -7},
    Pattern{0x0891b7dc, "C0 FF BD 27 ? ? B1 AF ? ? B2 AF 25 88 A0 00 25 90 C0 00", 0},
    Pattern{0x08911e6c, "00 00 12 34 ? ? ? ? 25 20 00 02 ? ? ? ? 25 20 00 02 ? ? 40 14 00 00 00 00 00 00 12 34 ? ? 40 12", 0},
    Pattern{0x08908080, "F0 FF BD 27 ? ? B1 AF FF FF 06 24", 0},
    Pattern{0x08906354, "25 80 80 00 ? ? ? ? 50 00 04 8e 50 00 04 8e ? ? ? ? bc 02 05 26 56 00 07 86", -4},
    Pattern{0x088f5a3c, "D0 FF BD 27 ? ? B0 AF ? ? B1 AF ? ? B2 AF ? ? B3 AF ? ? B4 AF ? ? B5 AF ? ? B6 AF ? ? BF AF FF 00 D2 30", 0},
    Pattern{0x088f47bc, "ff ff 09 24 3a 00 64 82 02 00 84 30", -4},
    Pattern{0x088809f8, "10 FE BD 27 ? ? B4 E7 ? ? B6 E7 ? ? B8 E7 ? ? BA E7 ? ? BC E7", 0},
    Pattern{0x08869b00, "25 20 c0 00 25 30 20 02 ? ? c0 10", -4},
    Pattern{0x088625e8, "F0 FF BD 27 ? ? BF AF ? ? 85 8F ? ? 86 8C"},
    Pattern{0x08bb2158, "? ? ? ? 80 28 05 00 21 20 85 00 00 00 84 8c ? ? ? ? 00 00 00 00 dc 01 24 92", 0, Pattern::GPOFFSET},
    Pattern{0x08aa2670, "? ? 86 8F 00 00 05 34 2A 30 86 00"},
    Pattern{0x08bb1ea8, "? ? 84 8F 25 28 00 00 ? ? BF AF", 0, Pattern::GPOFFSET},
    Pattern{0x08b42198, "F0 FF BD 27 ? ? B0 AF 25 80 80 00 ? ? 84 8F 80 28 10 00"},
    Pattern{0x08ad3a78,  "? ? ? ? 25 20 40 00 ? ? 40 14 00 00 00 00 ? ? ? ? 25 20 40 02 25 20 40 00", 0, Pattern::BRANCH},
    Pattern{0x08ae4efc, "56 00 44 84 ? ? ? ? ? ? 85 14 00 00 00 00 ? ? 00 10 01 00 04 34", 24},
    Pattern{0x089b3a20, "? ? ? ? 25 20 40 02 ? ? ? ? 25 20 40 02 ? ? 40 10 00 00 00 00 ? ? ? ? ? ? 05 3c 24 20 85 00"},
    Pattern{0x08ac53f8, "25 20 85 00 ? ? 80 14 00 00 00 00 ? ? ? ? 00 00 00 00 ? ? ? ? 00 00 00 00 ? ? 04 3c ? ? ? ? ? ? ? 24 ? ? ? ? 00 00 00 00", 40},
    Pattern{0x089f6b60, "? ? ? ? 25 20 80 02 ? ? ? ? 25 20 20 02 ? ? ? ? ? ? ? 27"},
    Pattern{0x089f6bac, "30 FF BD 27 ? ? B4 AF"},
    Pattern{0x08badd78, "? ? 85 8F 04 00 86 24", 0, Pattern::GPOFFSET},
    Pattern{0x08a65fe8, "? ? ? ? 00 00 00 00 ? ? 40 10 00 00 00 00 ? ? ? ? 01 00 84 24"},
    Pattern{0x08bb40d0, "? ? 8d c7 03 6b 0c 46 7a 44 04 3c", 0, Pattern::GPOFFSET},
    Pattern{0x08bb31b6, "01 00 84 24 ? ? ? ? ? ? ? ? 00 00 00 00 00 a0 80 44", -8, Pattern::GPOFFSET},
    Pattern{0x08ab5a3c, "? ? ? ? ? ? ? 24 25 20 40 00 ? ? 81 04"},
    Pattern{0x0882e2a8, "? ? ? ? 00 00 00 00 ? ? ? ? 00 00 00 00 ? ? 00 10 00 00 00 00 ? ? ? ? 25 20 00 02 ? ? 00 10 00 00 00 00 28 00 04 92"},
    Pattern{0x08b48060, "? ? ? ? 01 00 07 34 25 b0 e0 02"},
    Pattern{0x089edce0, "f0 ff bd 27 ? ? b0 af 25 80 80 00 ff 00 c6 30"},
    Pattern{0x089d9a48, "c0 ff bd 27 ? ? b0 af ? ? b3 af 25 98 a0 00"},
    Pattern{0x08832e04, "80 FF BD 27 ? ? B0 AF 25 80 80 00 B0 03 04 26"},
    Pattern{0x08835298, "05 00 04 34 ? ? 44 AE"},
    Pattern{0x08b45350, "f0 ff bd 27 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 00 00 00 00 ? ? ? ? f0 08 44 24 ? ? 40 10"},
    Pattern{0x08b49444, "? ? ? ? 25 20 c0 02 ? ? 00 10 00 00 00 00 ? ? ? ? 25 20 c0 02"},
    Pattern{0x0886589c, "1c 00 e4 48 1d 00 e5 48"},
    Pattern{0x08b84ab0, "ff 7f a5 30 ? ? 07 3c c0 30 05 00", 4, Pattern::UPPER_AND_ADD, 0xC},
    Pattern{0x08862524, "? ? ? ? 25 20 00 02 ? ? 40 10 00 00 00 00 ? ? 00 10 00 00 00 00 ? ? ? ? ? ? 80 10"},
    Pattern{0x08a53c74, "? ? [...00100] [100011..] ? ? [...00100] [100011..] ? ? 80 54 ? ? [...00100] [100011..] ? ? ? [000011..] 0 0 0 0 ? ? [...00100] [100011..] ? ? ? [000011..] ? ? [...00101] [100011..] ? ? [...00100] [100011..] ? ? 80 54", 28},
    Pattern{0x088f74ec, "A0 FF BD 27 ? ? B0 AF ? ? B1 AF ? ? BF AF FF FF 91 30"},
    Pattern{0x08960600, "? ? ? ? 00 00 00 00 ? ? 5e 54 ? ? ? ? ? ? ? ? 25 20 00 00", 0, Pattern::BRANCH},
    Pattern{0x08968930, "? ? ? ? 01 00 07 34 25 88 00 02 ? ? 20 12"},
    Pattern{0x08bb4084, "? ? 85 8f ? ? 85 ac 04 00 84 24 ? ? 85 8f", 0, Pattern::GPOFFSET},
    Pattern{0x08badd74, "? ? 84 8f ? ? 00 ae ? ? 84 8f", 0, Pattern::GPOFFSET},
    Pattern{0x08862890, "E0 FF BD 27 ? ? B4 AF 00 A4 06 00"},
    Pattern{0x08ad44c0, "? ? ? 0E 25 28 20 02 25 A0 40 00 ? ? 80 12"},
    Pattern{0x08ad8274, "90 FF BD 27 ? ? 85 93 ? ? B0 AF ? ? B1 AF ? ? B2 AF"},
    Pattern{0x08bb4050, "? ? 84 AF ? ? 85 90", 0, Pattern::GPOFFSET},
    Pattern{0x0896e7a8, "? ? 85 84 BF 00 A6 28"},
    Pattern{0x08bc77c8, "? ? 12 3c 25 28 20 02 01 00 06 34 ? ? ? ? ? ? ? ? ? ? ? 26 ? ? ? ? 04 63 00 46", 0, Pattern::UPPER_AND_ADD, 0x14},
    Pattern{0x08ae9210, "? ? 20 1a 28 00 25 2a ? ? a0 10"},
    Pattern{0x8a49344, "? ? ? ? 25 38 00 02 25 20 00 02"},
    Pattern{0x089da48c, "00 00 00 00 ? ? 01 45 ? ? ? ? ? ? ? ? 25 20 20 02 ? ? ? ? 25 20 40 00 ? ? 40 10 00 00 00 00 b3 3e 04 3c"},
    Pattern{0x089da494, "00 00 00 00 ? ? 01 45 ? ? ? ? ? ? ? ? 25 20 20 02 ? ? ? ? 25 20 40 00 ? ? 40 10 00 00 00 00 b3 3e 04 3c", 8},
    Pattern{0x08acbccc, "? ? ? ? 25 20 40 02 25 90 40 00 ff ff 04 24 ? ? 44 12"},
    Pattern{0x08aa1940, "? ? ? ? 25 80 80 00 25 a8 40 00 ? ? a0 12"},
    Pattern{0x089730dc, "f0 ff bd 27 ? ? ? ? 00 00 10 34 ? ? ? ? ? ? ? ? b0 00 04 34 25 20 40 00"},

};
