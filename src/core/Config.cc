#include <string>
#define CPPTOML_NO_RTTI

#include "Config.hh"
#include <limits>
#include "cpptoml/cpptoml.h"
#include "Common.hh"
#include "Logger.hh"
#include <string>

#ifdef ENABLE_DEBUG_MENU
#include <debug/config.hh>
#endif

/*******************************************************/
ConfigManager::ConfigManager (const std::string &file)
{
    try
        {
            m_pConfig = cpptoml::parse_file (
                Rainbomizer::Common::GetRainbomizerFileName (file));
        }
    catch (const std::exception &e)
        {
            Rainbomizer::Logger::LogMessage ("%s", e.what ());
            m_pConfig.reset ();
        }
}

/*******************************************************/
ConfigManager *
ConfigManager::GetInstance ()
{
    static ConfigManager mgr{"config.toml"};
    return &mgr;
}

/*******************************************************/
bool
ConfigManager::GetIsEnabled (const std::string &name)
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

    Rainbomizer::Logger::LogMessage ("%s: %s", name.c_str (),
                                     (enabled) ? "Yes" : "No");

    return enabled;
}

template <typename T>
void
ConfigManager::ReadValue (const std::string &tableName, const std::string &key,
                          T &out, bool tmp)
{
    if (!m_pConfig)
        return;

    auto table    = m_pConfig->get_table (tableName);

    if (table && table->contains (key))
        out = table->get_as<T> (key).value_or (out);

#ifdef DEBUG_CONFIG_OPTIONS
    std::ostringstream ss;
    ss << "\nConfig option: " << tableName << "." << key;
    ss << "\nChosen value : " << out;
    ss << "\nHas config table : " << table;
    ss << "\nHas default table : " << defTable;
    ss << "\nConfig has key: " << (table ? table->contains (key) : false);
    ss << "\nDefault has key: "
       << (defTable ? defTable->contains (key) : false);
    if (table && table->contains (key))
        ss << "\nConfig value: " << table->get_as<T> (key).value_or (out);
    if (defTable && defTable->contains (key))
        ss << "\nDefault Config value: "
           << defTable->get_as<T> (key).value_or (out);
    ss << "\n";

    Rainbomizer::Logger::LogMessage ("%s", ss.str ().c_str ());
#endif
}

#define READ_VALUE_ADD_TYPE(type)                                              \
    template void ConfigManager::ReadValue<type> (const std::string &,         \
                                                  const std::string &, type &, \
                                                  bool);

READ_VALUE_ADD_TYPE (bool)
READ_VALUE_ADD_TYPE (int)
READ_VALUE_ADD_TYPE (double)
READ_VALUE_ADD_TYPE (std::string)
