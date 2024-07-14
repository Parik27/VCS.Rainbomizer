#include "core/Common.hh"
#include "vcs/CText.hh"
#include <hooks/Hooks.hh>
#include <core/Randomizer.hh>
#include <core/Logger.hh>
#include <utils/Utils.hh>
#include <core/Config.hh>

class VoiceLineRandomizer : public Randomizer<VoiceLineRandomizer>
{
    static constexpr int SUBTITLE_BUFFER_SIZE = 128+3;

    struct VoiceLine
    {
        char           audioName[8] = {};
        bool           resolved     = false;
        std::u16string subtitle;
    };

    std::vector<VoiceLine> m_voiceLines;

    template <auto &cDMAudio__PreloadMissionAudioSlot>
    void
    RandomizeVoiceLine (void *p1, uint8_t slot, const char *audioName,
                        uint32_t p4, const char16_t *subtitle)
    {
        auto &line = GetRandomElementMut (m_voiceLines);
        cDMAudio__PreloadMissionAudioSlot (p1, slot, line.audioName, p4,
                                           line.subtitle.c_str ());
    }

    template <int N, typename T, typename U>
    bool
    StringCompare (T &a, U &b)
    {
        for (size_t i = 0; i < N; i++)
            {
                if (a[i] != b[i])
                    return false;
            }

        return true;
    }

    void
    ReadSubtitlesFromEntries (CKeyArray &keyArray)
    {
        int max = 0;
        for (auto &i : std::span(keyArray.entries, keyArray.numEntries))
            {
                for (auto &j : m_voiceLines) {

                        if (!j.resolved && StringCompare<7> (j.subtitle, i.key))
                            {
                                j.subtitle = i.value;
                                j.resolved = true;
                                break;
                            }
                }
            }
    }

    void
    ReadSubtitlesFromGxt (CText* text)
    {
        Rainbomizer::Logger::FunctionBenchmark benchmark;

        static constexpr std::array tables{
            "JER_A1", "JER_A2", "JER_A3",  "PHI_A1", "PHI_A2", "PHI_A3",
            "PHI_A4", "MAR_A1", "MAR_A2",  "MAR_A3", "MAR_A4", "MAR_A5",
            "LOU_A1", "LOU_A2", "LOU_A3",  "LOU_A4", "LOU_B1", "LOU_B2",
            "LAN_B1", "LAN_B2", "LAN_B4",  "LAN_B5", "LAN_B6", "LAN_C1",
            "LAN_C2", "LAN_C3", "LAN_C4",  "LAN_C6", "LAN_C5", "LAN_C7",
            "LAN_C8", "LAN_C9", "LAN_C10", "UMB_B1", "UMB_B2", "UMB_B3",
            "UMB_B4", "BRY_B1", "BRY_B3",  "BRY_B4", "REN_C1", "REN_C2",
            "REN_C3", "REN_C4", "REN_C5",  "REN_C6", "REN_C7", "GON_C2",
            "GON_C3", "GON_C4", "DIA_C1",  "DIA_C2", "DIA_C3", "DIA_C4",
            "DIA_C5", "MEN_C1", "MEN_C2",  "MEN_C3", "MEN_C5", "MEN_C6",
            "E_DR", "BRY3_85", "E_PR"
        };

        for (const auto &table : tables)
            {
                CText::LoadMissionText (text, table);
                ReadSubtitlesFromEntries (text->mission_keyArray);
            }
    }

    void
    LoadVoiceLines ()
    {
        Rainbomizer::Logger::FunctionBenchmark benchmark;

        m_voiceLines.clear ();

         auto f = Rainbomizer::Common::GetRainbomizerDataFile ("VoiceLines.txt");
        f.ReadLines ([this] (char *line) {
            VoiceLine voiceLine;
            int pos;

            sscanf (line, "%8s %n", voiceLine.audioName, &pos);
            line += pos;

            while (*line)
                voiceLine.subtitle.push_back (*line++);

            m_voiceLines.push_back (voiceLine);
        });
    }

    template <auto &CKeyArray__Update>
    void
    InitialiseSubtitles (CText *text, int chars)
    {
        // p1 is actually a CKeyArray, but it has the same address as CText
        // instance because of reasons

        ReadSubtitlesFromGxt (text);
        CKeyArray__Update (text, chars);
    }

public:
    VoiceLineRandomizer ()
    {
        RB_C_DO_CONFIG ("VoiceLineRandomizer");

        LoadVoiceLines ();

        if (m_voiceLines.size ())
            {
                HOOK_MEMBER (Jal, (0x08a8d730), RandomizeVoiceLine,
                             void (void *, uint8_t, const char *, uint32_t,
                                   const char16_t *));

                HOOK_MEMBER (Jal, (0x89f6b60), InitialiseSubtitles,
                             void (CText *, int));
            }
    }
};
