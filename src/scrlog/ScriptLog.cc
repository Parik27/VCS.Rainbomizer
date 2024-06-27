#include "core/Common.hh"
#include "core/Logger.hh"
#include "hooks/Hooks.hh"
#include "memory/GameAddress.hh"
#include "vcs/CRunningScript.hh"
#include <core/Randomizer.hh>
#include <cstdio>

class ScriptLog : public Randomizer<ScriptLog>
{
    struct OpcodeInfo
    {
        char name[128];
        char params[14] = {};
    };

    std::array<OpcodeInfo, 0x0565> opcodeInfo;

    FILE           *log;
    int16_t         currentOpcode = 0;
    int16_t         currentParam = 0;
    size_t          instructions_logged = 0;
    CRunningScript *prevScript = nullptr;
    CRunningScript *currScript = nullptr;

    void
    DumpLocalVariables ()
    {
        fputs ("Local variables dump:\n", log);
        for (auto i : currScript->m_aLocalVars)
            fprintf (log, "%d ", i);
        fputc ('\n', log);
    }

    void
    PrintHeader ()
    {
        fputs ("********************************************\n", log);
        fprintf (log, "script %s\n", currScript->m_szName);
        DumpLocalVariables ();
        fputs ("********************************************\n", log);
    }

    void
    PrintAddresAndOpcode (CRunningScript *scr)
    {
        uint16_t opcode
            = *(uint16_t *) &CTheScripts::ScriptSpace[scr->m_pCurrentIP];
        bool notFlag = (opcode & 0x8000) != 0;
        opcode       = opcode & 0x7FFF;
        for (int i = 0; i < scr->m_nSP; i++)
            fprintf(log, "\t");
        fprintf (log, "%08d&%d: [%08X] %s %s", scr->m_pCurrentIP,
                 scr->m_bCondResult, opcode, notFlag ? "NOT" : "",
                 opcodeInfo[opcode].name);

        currentOpcode = opcode;
        currentParam = 0;
    }

    void
    PrintInstructionEnd ()
    {
        fprintf (log, "\n");
    }

    template <auto &CRunningScript__ProcessOneCommand>
    int
    DoScriptLog (CRunningScript *scr)
    {
        if (!scr->m_bIsMission)
            return CRunningScript__ProcessOneCommand (scr);

        //ReopenFileIfItGotTooBig ();

        currScript = scr;
        if (prevScript != scr)
            PrintHeader ();
        PrintAddresAndOpcode (scr);
        int ret = CRunningScript__ProcessOneCommand (scr);
        PrintInstructionEnd ();
        prevScript = scr;
        return ret;
    }

    void
    ReopenFileIfItGotTooBig ()
    {
        if (instructions_logged++ % 500000 == 0)
            {
                fclose (log);
                log = Rainbomizer::Common::GetRainbomizerFile ("scrlog.log",
                                                               "a+")
                          .Release ();
            }
    }

    void
    ReadOpcodesInfo ()
    {
        auto f = Rainbomizer::Common::GetRainbomizerFile ("opcodes.txt", "r");
        if (!f)
            return;

        char line[256];
        while (fgets (line, sizeof (line), f))
            {
                line[strcspn (line, "\n")] = 0;
                int  opcode = 0;
                char name[128] = {};
                char params[14] = {};
                if (sscanf (line, "%x %s %s", &opcode, name, params) >= 2)
                    {
                        strcpy (opcodeInfo[opcode].name, name);
                        strcpy (opcodeInfo[opcode].params, params);
                    }
            }
    }

    template<auto &CRunningScript__CollectParams>
    int DumpParams (CRunningScript* scr, int* ip, int count, int* params)
    {
        int ret = CRunningScript__CollectParams (scr, ip, count, params);

        if (scr != currScript)
            return ret;

        for (size_t i = 0; i < count; i++)
            {
                switch (opcodeInfo[currentOpcode].params[currentParam + i])
                    {
                    case 'f':
                        fprintf (log, " %f", *(float *) &params[i]);
                        break;
                    case 's':
                        fprintf (log, " %s",
                        CTheScripts::ScriptSpace + params[i]);
                        break;
                    case 'i':
                    default: fprintf (log, " %d", params[i]); break;
                    }
            }

        currentParam += count;

        return ret;
    }

    template<auto &CRunningScript__StoreParameters>
    void DumpReturn (CRunningScript* scr, int* pc, int count)
    {
        static auto ScriptParams = GameVariable<int[20], 0x8bc77c8>{};

        CRunningScript__StoreParameters (scr, pc, count);

        if (scr != currScript)
            return;

        fprintf (log, " ->");
        for (size_t i = 0; i < count; i++)
            {
                switch (opcodeInfo[currentOpcode].params[currentParam + i])
                    {
                    case 'f':
                        fprintf (log, " %f", *(float *) &ScriptParams[i]);
                        break;
                    case 'i':
                    default: fprintf (log, " %d", ScriptParams[i]);
                        break;
                    }
            }

        currentParam = 0;

        return;
    }

public:
    ScriptLog ()
    {

        log = Rainbomizer::Common::GetRainbomizerFile ("scrlog.log", "a+").Release();
        ReadOpcodesInfo ();
        HOOK_MEMBER (Jal, 0x8862524, DoScriptLog, int (CRunningScript *));
        HOOK_MEMBER (Jmp, 0x0886589c, DumpParams,
                     int (CRunningScript *, int *, int, int *));
        HOOK_MEMBER (Jmp, 0x08862890, DumpReturn,
                     void (CRunningScript *, int*, int));
    }
};
