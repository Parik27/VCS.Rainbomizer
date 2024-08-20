#include "weapon/WeaponPatterns.hh"
#include "core/Common.hh"
#include "core/Logger.hh"
#include "core/ThreadUtils.hh"
#include "utils/Random.hh"
#include "vcs/CRunningScript.hh"
#include "vcs/CWeaponInfo.hh"
#include <bitset>
#include <ranges>

#include "WeaponGroups.hh"
#include "WeaponGroupList.hh"

#ifdef ENABLE_DEBUG_MENU
#include <debug/base.hh>
#include <imgui.h>
#endif

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

template <typename T>
inline constexpr void
WeaponPattern::ReadFlag (std::string_view data, std::string_view flagName,
                         T &out)
{
    if (data.starts_with (flagName)
        && data.substr (flagName.size ()).starts_with ("="))
        std::from_chars (data.data () + flagName.size () + 1,
                         data.data () + data.size (), out);
}

void
WeaponPattern::ReadFlag (std::string_view flag)
{
    ReadFlag (flag, "min_x", m_RegionCheck.MinX);
    ReadFlag (flag, "min_y", m_RegionCheck.MinY);
    ReadFlag (flag, "max_x", m_RegionCheck.MaxX);
    ReadFlag (flag, "max_y", m_RegionCheck.MaxY);
    ReadFlag (flag, "override_ammo", m_OverrideAmmo);
}

void
WeaponPattern::ReadFlags (const char *flags)
{
    Rainbomizer::Logger::LogMessage ("Reading weapon randomizer flags: %s",
                                     flags);

    std::string_view flags_sv (flags);
    for (auto flag : std::views::split (flags_sv, '+'))
        {
            ReadFlag (std::string_view (flag));
        }
}

void
WeaponPattern::Read (const char *line)
{
    char originalWeapon[32] = {};
    char pattern[256]       = {};
    char flags[256]         = {};

    sscanf (line, "%s %d %d %d %[^\t\n] %s", originalWeapon, &m_Mission,
            &m_Ammo, &m_Ped, pattern, flags);

    ReadFlags (flags);

    std::array<float, WEAPON_NUM_WEAPONS> weights;

    GetWeaponGroup ("all").ForEachWeapon (
        [&weights] (int weapon) { weights[weapon] = 1.0f; });

    m_WeaponGroup = &GetWeaponGroup (originalWeapon);

    for (auto i : std::ranges::split_view (pattern, ' '))
        {
            auto split_view = std::ranges::split_view (i, '=');

            if (std::ranges::distance (split_view) != 2)
                continue;

            auto it      = split_view.begin ();
            auto type_s  = std::string_view (*it);
            auto value_s = std::string_view (*++it);

            float weight;
            std::from_chars (value_s.data (), value_s.data () + value_s.size (),
                             weight);

            auto group = GetWeaponGroup (type_s);
            group.ForEachWeapon ([this, &weights, weight] (int weapon) {
                weights[weapon] = weight;
            });

            Rainbomizer::Logger::LogMessage ("%s", line);
            for (auto [weight, idx] :
                 std::views::zip (weights, std::views::iota (0)))
                {
                    Rainbomizer::Logger::LogMessage ("%d = %f", idx, weight);
                }
        }

    m_Distribution
        = std::discrete_distribution<int> (weights.begin (), weights.end ());
}

void
WeaponPattern::GetRandom (Result &result)
{
    result.Weapon       = m_Distribution (RandEngine ());
    result.OverrideAmmo = m_OverrideAmmo;
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

bool
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
                    return true;
                }
        }

    pattern.Weapon = weaponType;
    return false;
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

    if (m_Ped > 0 && ped && ped->m_nModelIndex != m_Ped)
        return false;

    // Region check
    if (m_RegionCheck.MaxX != -1 && m_RegionCheck.MaxY != -1
        && m_RegionCheck.MinX != -1 && m_RegionCheck.MinY != -1 && ped)
        {
            const CVector &pos = ped->vecPosition;
            if (pos.x < m_RegionCheck.MinX || pos.x > m_RegionCheck.MaxX
                || pos.y < m_RegionCheck.MinY || pos.y > m_RegionCheck.MaxY)
                return false;
        }

    return true;
}

void
WeaponPattern::DrawDebugInfo ()
{
#ifdef ENABLE_DEBUG_MENU
    ImGui::TableNextColumn ();
    ImGui::Text ("%d", m_Mission);
    ImGui::TableNextColumn ();
    ImGui::Text ("%d", m_Ammo);
    ImGui::TableNextColumn ();
    ImGui::Text ("%d", m_Ped);
    ImGui::TableNextColumn ();
    ImGui::Text ("%d %d %d %d", m_RegionCheck.MinX, m_RegionCheck.MinY,
                 m_RegionCheck.MaxX, m_RegionCheck.MaxY);
    ImGui::TableNextColumn ();
    ImGui::Text ("%d", m_OverrideAmmo);
#endif
}

void
WeaponPatternManager::DrawDebugInfo ()
{
#ifdef ENABLE_DEBUG_MENU
    ImGui::BeginTable ("Weapon Patterns", 6);
    ImGui::TableSetupColumn ("Mission");
    ImGui::TableSetupColumn ("Ammo");
    ImGui::TableSetupColumn ("Ped");
    ImGui::TableSetupColumn ("Region");
    ImGui::TableSetupColumn ("Override Ammo");
    ImGui::TableHeadersRow ();
    for (auto &p : m_aPatterns)
        {
            ImGui::TableNextRow();
            p.DrawDebugInfo ();
        }
    ImGui::EndTable ();
#endif
}
