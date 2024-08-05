#pragma once

#include "vcs/CRunningScript.hh"
#include "vcs/CVector.hh"
#include "vcs/CModelInfo.hh"
#include "vcs/CVehicle.hh"

#include <cstdint>
#include <vector>
#include <bitset>

#include "VehicleGroups.hh"

class ScriptVehiclePattern
{
    struct VehicleTypes
    {
        bool Cars : 1;
        bool Bikes : 1;
        bool Bicycles : 1;
        bool Quadbikes : 1;
        bool Planes : 1;
        bool Helicopters : 1;
        bool Boats : 1;

        bool GetValue (eVehicleType type) const;

    } mAllowedTypes, mMovedTypes;

    struct
    {
        bool m_bMovedCoordsAreAbsolute : 1 = false;
        bool m_bNotOriginal : 1            = false;
        bool m_bDontMoveBovver : 1         = false;
    } m_Flags;
    CVector m_vecMovedCoords          = {0.0, 0.0, 0.0};
    CVector m_vecBoundsCheck          = {1000.0f, 1000.0f, 1000.0f};

    uint32_t m_nSeatCheck       = 0;
    uint32_t m_nOriginalVehicle = 0;
    uint16_t m_Mission          = 0;

    std::bitset<std::tuple_size_v<decltype (s_VehicleGroups)>>
        m_aIncludedGroups = 0;

    std::bitset<std::tuple_size_v<decltype (s_VehicleGroups)>>
        m_aExcludedGroups = 0;

    int16_t m_posX = 0;
    int16_t m_posY = 0;
    int16_t m_posZ = 0;
    int16_t m_ForcedVehicle = -1;

    std::vector<uint16_t> m_BlacklistedVehicles;
    std::string m_OriginalLine;

#ifdef USE_CACHE
    bool                  m_bCached = false;
    std::vector<uint32_t> m_aCache;
#endif

    void
    InvalidateCache ()
    {
#ifdef USE_CACHE
        m_bCached = false;
#endif
    }

public:
    struct Result
    {
        int            vehId          = -1;
        bool           absoluteCoords = false;
        const CVector *coords         = nullptr;
        const CVector *boundsCheck    = nullptr;
    };

    template <size_t>
    inline constexpr bool
    DoesVehicleSatisfyGroupRequirement (eVehicle id) const;
    inline constexpr bool
    DoesVehicleSatisfyGroupRequirements (eVehicle id) const;

    bool IsValidVehicleForPattern (eVehicle id) const;
    void Cache ();

    void GetRandom (Result &result) const;
    void GetRandomLoaded (Result &result) const;

    bool Match (uint32_t hash, const CVector &pos,
                CRunningScript *script) const;

    template <size_t I>
    inline constexpr void ReadVehicleGroupFlag (std::string_view flag);

    template <typename T>
    inline constexpr void ReadFlag (std::string_view data,
                                    std::string_view flagName, T &out);

    inline constexpr void ReadFlag (std::string_view flag);

    void ReadBlacklist (const char *line);
    void ReadFlags (const char *line);
    void Read (const char *line);

    void LogPattern () const;
};

class VehiclePatternManager
{
    std::vector<ScriptVehiclePattern> m_aPatterns;

public:
    void ReadPatterns (const char *file);
    void GetRandomVehicle (eVehicle original, CRunningScript *script,
                           const CVector &pos, ScriptVehiclePattern::Result &result);

    void LogAllPatterns ();
};
