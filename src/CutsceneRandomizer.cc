#include "Hooks.hh"
#include "core/Randomizer.hh"
#include "log.h"
#include <cstdlib>
#include <iterator>
#include <set>
#include <string>

BEGIN_RANDOMIZER (CutsceneRandomizer)

std::set<std::string> m_Names;

public:
template <auto &strtok>
const char *
RandomizeCutsceneObject (char *dst, char *src)
{
    const char *origModel = strtok (dst, src);
    m_Names.insert(origModel);

    return std::next(m_Names.begin(), rand() % m_Names.size())->c_str();
    return origModel;
}

CutsceneRandomizer ()
{
    REGISTER_HOOK_MEMBER_ADDR (0x8b3d0a0, RandomizeCutsceneObject, const char *,
                               char *, char *);
}
END_RANDOMIZER (CutsceneRandomizer)
