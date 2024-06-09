#include "Common.hh"
#include "log.h"
#include <type_traits>
#define TOML_IMPLEMENTATION

#include "Config.hh"
#include "Stream.hh"

ConfigManager::ConfigManager ()
{
    auto f = Rainbomizer::Common::OpenFileForReading ("config.toml");
    if (!f)
        {
            logger.WriteF ("Failed to open config file for reading");
        }

    m_Config = toml::parse (f);

    if (!m_Config)
        {
            logger.WriteF ("Failed to read config file: %s (at line %d)",
                           m_Config.error ().description ().data(),
                           m_Config.error ().source ().begin.line);
        }
}

/*******************************************************/
bool
ConfigManager::GetIsEnabled (const char* name)
{
    // Finds "name" key in the main table. Also allows an "Enabled" key in the
    // table for the randomizer/whatever.

    // Example:
    // TrafficRandomizer = true
    // ColourRandomizer = false
    // [ColourRandomizer]
    // Enabled = true

    // Will be parsed as TrafficRandomizer and ColourRandomizer enabled.
    // Enabled key takes precedence over main table key.

    bool enabled = true;
    ReadValue ("Randomizers", name, enabled, true);
    ReadValue (name, "Enabled", enabled, true);

    logger.WriteF ("%s: %s", name, (enabled) ? "Yes" : "No");

    return enabled;
}

template <typename T>
void
ConfigManager::ReadValue (const std::string &tableName, const std::string &key,
                          T &out, bool tmp)
{
    out = m_Config[tableName][key].value_or (out);
}

#define READ_VALUE_ADD_TYPE(type)                                              \
    template void ConfigManager::ReadValue<type> (const std::string &,         \
                                                  const std::string &, type &, \
                                                  bool);

READ_VALUE_ADD_TYPE (bool)
READ_VALUE_ADD_TYPE (int)
READ_VALUE_ADD_TYPE (double)
READ_VALUE_ADD_TYPE (std::string)
