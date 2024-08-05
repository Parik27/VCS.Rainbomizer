#include "vehicle/VehiclePatterns.hh"
#include "core/Common.hh"
#include "core/Logger.hh"
#include "core/ThreadUtils.hh"
#include "utils/Random.hh"
#include "vcs/CKeyGen.hh"
#include "vcs/CRunningScript.hh"
#include "vcs/CStreaming.hh"
#include "vcs/CVehicle.hh"
#include "vehicle/Common.hh"
#include <cstdio>
#include "VehicleGroups.hh"

constexpr int STREAMING_THRESHOLD = 17;

bool
ScriptVehiclePattern::VehicleTypes::GetValue (eVehicleType type) const
{
    switch (type)
        {
        case VEHICLE_TYPE_AUTOMOBILE: return Cars;
        case VEHICLE_TYPE_BOAT: return Boats;
        case VEHICLE_TYPE_JETSKI: return Boats;
        case VEHICLE_TYPE_HELI: return Helicopters;
        case VEHICLE_TYPE_PLANE: return Planes;
        case VEHICLE_TYPE_BIKE: return Bikes;
        case VEHICLE_TYPE_BMX: return Bicycles;
        case VEHICLE_TYPE_QUAD: return Quadbikes;
        default: return false;
        }
}

template <size_t I>
constexpr void
ScriptVehiclePattern::ReadVehicleGroupFlag (std::string_view flag)
{
    constexpr auto &group = std::get<I> (s_VehicleGroups);
    if (flag == group.name)
        m_aIncludedGroups[I] = true;

    else if (flag.starts_with ("Not") && flag.substr (3) == group.name)
        m_aExcludedGroups[I] = true;
}

template <typename T>
inline constexpr void
ScriptVehiclePattern::ReadFlag (std::string_view data,
                                std::string_view flagName, T &out)
{
    if (data.starts_with (flagName)
        && data.substr (flagName.size ()).starts_with ("="))
        std::from_chars (data.data () + flagName.size () + 1,
                         data.data () + data.size (), out);
}

constexpr void
ScriptVehiclePattern::ReadFlag (std::string_view flag)
{
    // Vehicle Group Flags
    [this, flag]<std::size_t... I> (std::index_sequence<I...>) {
        (..., ReadVehicleGroupFlag<I> (flag));
    }(std::make_index_sequence<
        std::tuple_size_v<decltype (s_VehicleGroups)>>{});

    // Read position check
    ReadFlag (flag, "x", m_posX);
    ReadFlag (flag, "y", m_posY);
    ReadFlag (flag, "z", m_posZ);
    ReadFlag (flag, "ForcedVehicle", m_ForcedVehicle);

    if (flag == "AbsAltCoords")
        m_Flags.m_bMovedCoordsAreAbsolute = true;

    if (flag == "NotOriginal")
        m_Flags.m_bNotOriginal = true;

    if (flag == "DontMoveBovver")
        m_Flags.m_bDontMoveBovver = true;
}

void
ScriptVehiclePattern::ReadFlags (const char *line)
{
    auto flags = std::string_view (line);
    for (auto flag : std::views::split (flags, '+'))
        {
            ReadFlag (std::string_view (flag));
        }
}

void
ScriptVehiclePattern::Read (const char *line)
{
    char vehicleName[64] = {0};
    char flags[256]      = {0};
    char blacklist[256]  = {0};
    char cars            = 'N';
    char bikes           = 'N';
    char bicycles        = 'N';
    char quadbikes       = 'N';
    char planes          = 'N';
    char helicopters     = 'N';
    char boats           = 'N';

    sscanf (line, "%hu %s %d %c %c %c %c %c %c %c %s %f %f %f %s", &m_Mission,
            vehicleName, &m_nSeatCheck, &cars, &bikes, &bicycles, &quadbikes,
            &planes, &helicopters, &boats, flags, &m_vecMovedCoords.x,
            &m_vecMovedCoords.y, &m_vecMovedCoords.z, blacklist);

    m_nOriginalVehicle = CKeyGen::GetUppercaseKey (vehicleName);
    ReadFlags (flags);
    ReadBlacklist (blacklist);

    mAllowedTypes
        = {cars == 'Y',   bikes == 'Y',       bicycles == 'Y', quadbikes == 'Y',
           planes == 'Y', helicopters == 'Y', boats == 'Y'};

    mMovedTypes
        = {cars == 'C',   bikes == 'C',       bicycles == 'C', quadbikes == 'C',
           planes == 'C', helicopters == 'C', boats == 'C'};

    m_OriginalLine = line;
}

void
ScriptVehiclePattern::LogPattern () const
{
    auto patternFilter
        = [this] (int id) { return IsValidVehicleForPattern (eVehicle (id)); };

    static char patternLog[4096] = {};
    patternLog[0]                = '\0';

    Rainbomizer::Logger::LogMessage("== %s ==", m_OriginalLine.c_str());

    for (auto i : VehicleCommon::AllUsableVehicles() | std::views::filter (patternFilter))
        {
            auto *info = ModelInfo::GetModelInfo<CVehicleModelInfo> (i);
            sprintf (patternLog, "%s %s", patternLog, info->m_sName);
        }

    Rainbomizer::Logger::LogMessage("Allowed: %s", patternLog);
}

void
ScriptVehiclePattern::GetRandom (Result &result) const
{
    auto patternFilter
        = [this] (int id) { return IsValidVehicleForPattern (eVehicle (id)); };
    bool streamingOverloaded
        = CStreaming::sm_Instance->m_numVehiclesLoaded > STREAMING_THRESHOLD;

    if (streamingOverloaded)
        {
            result.vehId
                = GetRandomElement (VehicleCommon::LoadedUsableVehicles ()
                                    | std::views::filter (patternFilter));
        }
    else
        {
            result.vehId
                = GetRandomElement (VehicleCommon::AllUsableVehicles ()
                                    | std::views::filter (patternFilter));
        }

    if (m_ForcedVehicle != -1)
        result.vehId = m_ForcedVehicle;

    auto *info = ModelInfo::GetModelInfo<CVehicleModelInfo> (result.vehId);
    if (mMovedTypes.GetValue (info->m_vehicleType)
        && !(result.vehId == VEHICLE_HOVERCR && m_Flags.m_bDontMoveBovver))
        {
            result.coords         = &m_vecMovedCoords;
            result.absoluteCoords = m_Flags.m_bMovedCoordsAreAbsolute;
        }
}

bool
ScriptVehiclePattern::Match (uint32_t hash, const CVector &pos,
                             CRunningScript *script) const
{
    if (ThreadUtils::GetMissionIdFromThread (script) != m_Mission)
        return false;

    if ((m_posX != 0 && static_cast<int16_t> (pos.x) != m_posX)
        || (m_posY != 0 && static_cast<int16_t> (pos.y) != m_posY)
        || (m_posZ != 0 && static_cast<int16_t> (pos.z) != m_posZ))
        return false;

    return m_nOriginalVehicle == hash;
}

void
ScriptVehiclePattern::ReadBlacklist (const char *line)
{
    std::string_view blacklist (line);
    for (auto id : std::views::split (blacklist, ','))
        {
            size_t vehicleId;
            std::from_chars (id.begin (), id.end (), vehicleId);
            m_BlacklistedVehicles.push_back (vehicleId);
        }
}

template <size_t I>
constexpr bool
ScriptVehiclePattern::DoesVehicleSatisfyGroupRequirement (eVehicle id) const
{
    constexpr auto &group = std::get<I> (s_VehicleGroups);

    if (m_aIncludedGroups[I] && !DoesElementExist (group.vehicles, id))
        return false;

    if (m_aExcludedGroups[I] && DoesElementExist (group.vehicles, id))
        return false;

    return true;
}

constexpr bool
ScriptVehiclePattern::DoesVehicleSatisfyGroupRequirements (eVehicle id) const
{
    return [this, id]<std::size_t... I> (std::index_sequence<I...>) {
        return (... && DoesVehicleSatisfyGroupRequirement<I> (id));
    }(std::make_index_sequence<
               std::tuple_size_v<decltype (s_VehicleGroups)>>{});
}

bool
ScriptVehiclePattern::IsValidVehicleForPattern (eVehicle id) const
{
    auto model   = ModelInfo::GetModelInfo<CVehicleModelInfo> (id);
    auto vehType = model->m_vehicleType;

    // Original vehicle is always valid
    if (model->m_hashName == m_nOriginalVehicle)
        return !m_Flags.m_bNotOriginal;

    auto seats
        = CVehicleModelInfo::GetMaximumNumberOfPassengersFromNumberOfDoors (id)
          + 1;

    if (seats < m_nSeatCheck)
        return false;

    if (!DoesVehicleSatisfyGroupRequirements (id))
        return false;

    if (DoesElementExist (m_BlacklistedVehicles, id))
        return false;

    return mAllowedTypes.GetValue (vehType) || mMovedTypes.GetValue (vehType);
}

void
VehiclePatternManager::ReadPatterns (const char *file)
{
    m_aPatterns.clear ();
    auto f = Rainbomizer::Common::GetRainbomizerDataFile (file, "r");

    f.ReadLines ([this] (const char *line) {
        if (line[0] == '#' || strlen (line) < 10)
            return;

        ScriptVehiclePattern pattern;
        pattern.Read (line);

        m_aPatterns.push_back (std::move (pattern));
    });

    Rainbomizer::Logger::LogMessage ("Initialised %d patterns from %s",
                                     m_aPatterns.size (), file);
}

void
VehiclePatternManager::GetRandomVehicle (eVehicle                      original,
                                         CRunningScript               *script,
                                         const CVector                &pos,
                                         ScriptVehiclePattern::Result &result)
{
    auto model
        = ModelInfo::GetModelInfo<CVehicleModelInfo> (original)->m_hashName;

    size_t patternId = 0;
    for (const auto &i : m_aPatterns)
        {
            if (i.Match (model, pos, script))
                {
                    Rainbomizer::Logger::LogMessage (
                        "Vehicle %d matched pattern %zu", original, patternId);
                    i.GetRandom (result);
                    return;
                }

            patternId++;
        }

    if (CStreaming::sm_Instance->m_numVehiclesLoaded < STREAMING_THRESHOLD)
        result.vehId = VehicleCommon::GetRandomUsableVehicle ();
    else
        {
            Rainbomizer::Logger::LogMessage (
                "Too many vehicles loaded!, using loaded vehicles pool");

            result.vehId = VehicleCommon::GetRandomUsableLoadedVehicle ();
        }
}

void
VehiclePatternManager::LogAllPatterns ()
{
    for (const auto &i : m_aPatterns)
        i.LogPattern ();
}
