#pragma once

#include "vcs/CPlayer.hh"
#include "weapon/WeaponGroups.hh"
#include <random>
#include <vcs/CPed.hh>
#include <vector>

class WeaponPattern
{
    std::discrete_distribution<> m_Distribution;
    int                          m_Mission;
    const WeaponGroup           *m_WeaponGroup;
    int                          m_Ammo;
    int                          m_Ped;

    struct RegionCheck
    {
        int MinX = -1, MinY = -1, MaxX = -1, MaxY = -1;
    } m_RegionCheck;

    template <typename T>
    inline constexpr void ReadFlag (std::string_view data,
                                    std::string_view flagName, T &out);

    void ReadFlag (std::string_view flag);
    void ReadFlags (const char *flags);

public:
    struct Result
    {
        int Weapon;
    };

    bool Match (CPed *ped, int mission, int weaponType, int ammo);

    void GetRandom (Result &result);

    void Read (const char *line);
};

class WeaponPatternManager
{
    std::vector<WeaponPattern> m_aPatterns;

public:
    void ReadPatterns (const char *file);
    bool GetRandomWeapon (CPed *ped, int weaponType, int ammo,
                          WeaponPattern::Result &pattern);
};
