#include <string>

#include <memory/Memory.hh>
#include <utils/Random.hh>
#include <utils/Utils.hh>

#include "core/Config.hh"
#include "core/Randomizer.hh"
#include "core/Logger.hh"

class PlayerRandomizer : public Randomizer<PlayerRandomizer>
{
    static constexpr const char *m_cPlayerSkins[]
        = {"plr",     "cop",    "swat",   "fbi",     "army",    "medic",
           "fireman", "male01", "HFYST",  "HFOST",   "HMYST",   "HMOST",
           "HFYRI",   "HFORI",  "HMYRI",  "HMORI",   "HFYBE",   "HFOBE",
           "HMYBE",   "HMOBE",  "HFYBU",  "HFYMD",   "HFYCG",   "HFYPR",
           "HFOTR",   "HMOTR",  "HMYAP",  "HMOCA",   "BMODK",   "BMYCR",
           "BFYST",   "BFOST",  "BMYST",  "BMOST",   "BFYRI",   "BFORI",
           "BMYRI",   "BFYBE",  "BMYBE",  "BFOBE",   "BMOBE",   "BMYBU",
           "BFYPR",   "BFOTR",  "BMOTR",  "BMYPI",   "BMYBB",   "WMYCR",
           "WFYST",   "WFOST",  "WMYST",  "WMOST",   "WFYRI",   "WFORI",
           "WMYRI",   "WMORI",  "WFYBE",  "WMYBE",   "WFOBE",   "WMOBE",
           "WMYCW",   "WFOGO",  "WMOGO",  "WFYLG",   "WMYLG",   "WFYBU",
           "WMYBU",   "WMOBU",  "WFYPR",  "WFOTR",   "WMOTR",   "WMYPI",
           "WMOCA",   "WFYJG",  "WMYJG",  "WFYSK",   "WMYSK",   "WFYSH",
           "WFOSH",   "JFOTO",  "JMOTO",  "PGang7A", "PGang7B", "Gang1A",
           "Gang1B",  "Gang2A", "Gang2B", "Gang3A",  "Gang3B",  "Gang4A",
           "Gang4B",  "Gang5A", "Gang5B", "Gang6A",  "Gang6B",  "Gang7A",
           "Gang7B",  "Gang8A", "Gang8B", "Gang9A",  "Gang9B",  "DiaGgA",
           "DiaGgB",  "GonGgA", "GonGgB", "MenGgA",  "MenGgB",  "DEA1",
           "DEA2",    "vice1",  "vice2",  "BCop",    "Alberto", "Arman",
           "Barry",   "Burger", "Bry",    "BryB",    "BryE",    "Cokhed",
           "Diaz",    "Diego",  "FlmCrw", "Gtr1",    "Gonz",    "Hitman",
           "Gtr2",    "Jerry",  "LanSm",  "lanst",   "Lou",     "PhCol2",
           "LouB",    "LouM",   "Marty",  "ShpKpr3", "GayBiA",  "GayBiB",
           "PhCol",   "Phil",   "Reni",   "RenOp",   "Robber",  "Sarge",
           "ShopKpr", "umber",  "ZomA",   "plr2",    "Plr3",    "Plr4",
           "Plr5",    "Plr6",   "Plr7",   "Plr8",    "Plr9",    "Plr10",
           "Plr11",   "Plr12",  "Plr13",  "plr14"};

    // config
    std::string ForcedModel;

    template <auto &CPed__Undress>
    static void
    RandomizePed (class CPed *ped, const char *outfit)
    {
        const char *newOutfit = GetRandomElement (m_cPlayerSkins);
        Rainbomizer::Logger::LogMessage ("outfit: %s", newOutfit);
        CPed__Undress (ped, newOutfit);
    }

    template <auto &CollectParams>
    static void
    RandomizePlayer (class CRunningScript *scr, int *p1, int p2, int *p3)
    {
        CollectParams (scr, p1, p2, p3);
    }

public:
    PlayerRandomizer ()
    {
        RB_C_DO_CONFIG ("PlayerRandomizer", ForcedModel);

        HOOK (Jal, (0x08b2a76c), RandomizePed,
              void (class CPed *, const char *));

        HOOK (Jal, (0x8968524), RandomizePed,
              void (class CPed *, const char *));

        HOOK (Jal, (0x089de070), RandomizePlayer,
              void (class CRunningScript *, int *, int, int *));
    }
} g_playerRando;
