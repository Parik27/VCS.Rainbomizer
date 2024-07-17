#include "weapon/WeaponPatterns.hh"
#include "core/Common.hh"
#include "core/Logger.hh"
#include "core/ThreadUtils.hh"
#include "utils/Random.hh"
#include "vcs/CRunningScript.hh"
#include "vcs/CWeaponInfo.hh"
#include <bitset>

#include "WeaponGroups.hh"
#include "WeaponGroupList.hh"

// The number of groups are short enough and a map
// would be dynamically allocated so just using a
// static array and also I already wrote this code
// and there will be static initialization order
// fiasco with a static map so this is an array
// end of discussion
constexpr const WeaponGroup &
GetWeaponGroup (std::string_view name)
{
    for (const auto &group : s_WeaponGroups)
        {
            if (group.Name == name)
                {
                    return group;
                }
        }

    return s_WeaponGroups[0];
}

void
WeaponPattern::Read (const char *line)
{
    char originalWeapon[32] = {};
    char pattern[256]       = {};

    sscanf (line, "%s %d %d %d %[^\n]", originalWeapon, &m_Mission, &m_Ammo,
            &m_Ped, pattern);

    std::array<float, WEAPON_NUM_WEAPONS> weights;

    GetWeaponGroup ("all").ForEachWeapon (
        [&weights] (int weapon) { weights[weapon] = 1.0f; });

    m_WeaponGroup = &GetWeaponGroup (originalWeapon);

    for (auto i : std::ranges::split_view (pattern, ' '))
        {
            auto type    = std::ranges::split_view (i, '=').begin ();
            auto type_s  = std::string_view (*type);
            auto value_s = std::string_view (*++type);

            float weight;
            std::from_chars (value_s.data (), value_s.data () + value_s.size (),
                             weight);

            Rainbomizer::Logger::LogMessage ("%s", type_s.data ());

            auto group = GetWeaponGroup (type_s);
            group.ForEachWeapon ([this, &weights, weight] (int weapon) {
                weights[weapon] = weight;
            });
        }

    m_Distribution
        = std::discrete_distribution<int> (weights.begin (), weights.end ());
}

void
WeaponPattern::GetRandom (Result &result)
{
    result.Weapon = m_Distribution (RandEngine ());
}

void
WeaponPatternManager::ReadPatterns (const char *file)
{
    auto f = Rainbomizer::Common::GetRainbomizerDataFile (file);
    f.ReadLines ([this] (const char *line) {
        WeaponPattern pattern;
        pattern.Read (line);
        m_aPatterns.push_back (std::move (pattern));
    });
}

void
WeaponPatternManager::GetRandomWeapon (CPed *ped, int weaponType, int ammo,
                                       WeaponPattern::Result &pattern)
{
    int currentMission = -2;

    for (auto &p : m_aPatterns)
        {
            if (CTheScripts::CurrentScript)
                currentMission = ThreadUtils::GetMissionIdFromThread (
                    CTheScripts::CurrentScript);

            if (p.Match (ped, currentMission, weaponType, ammo))
                {
                    p.GetRandom (pattern);
                    return;
                }
        }

    pattern.Weapon = weaponType;
}

bool
WeaponPattern::Match (CPed *ped, int mission, int weaponType, int ammo)
{
    if (m_Mission != -1 && m_Mission != mission)
        return false;

    if (m_WeaponGroup && !m_WeaponGroup->CheckWeapon (weaponType))
        return false;

    if (m_Ammo != -1 && m_Ammo != ammo)
        return false;

    // for player only allow player patterns
    if (FindPlayerPed () == ped && m_Ped != 0)
        return false;

    if (m_Ped == 0 && ped != FindPlayerPed ())
        return false;

    return true;
}
