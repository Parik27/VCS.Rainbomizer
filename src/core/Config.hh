#pragma once

#include "inireader.h"
#include "ConfigMacros.hh"

#include <string>
#include "toml++/toml.hpp"

/*******************************************************/
class ConfigManager
{
    decltype(toml::parse_file("")) m_Config{};

    ConfigManager ();

    template <typename T>
    void ReadValue (const std::string &tableName, const std::string &key,
                    T &out, bool tmp = false);

    bool GetIsEnabled (const char *name);

public:
    /// Returns the static instance for ConfigManager.
    static ConfigManager* Get ()
    {
        static ConfigManager config;
        return &config;
    }

    template <typename... Args>
    static bool
    ReadConfig (const char *table, Args... params)
    {
        if (!Get ()->GetIsEnabled (table))
            return false;

        (Get ()->ReadValue (table, params.first, *params.second), ...);
        return true;
    }
};
