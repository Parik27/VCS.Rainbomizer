#pragma once

#include <string>
#include <memory>
#include <vector>
#include "ConfigMacros.hh"

namespace cpptoml {
class table;
} // namespace cpptoml

template <typename T>
concept HasCustomDebugInput = requires (T::value_type a) { T::DebugInput (&a); };

/*******************************************************/
class ConfigManager
{

    std::shared_ptr<cpptoml::table> m_pConfig;
    std::shared_ptr<cpptoml::table> m_pDefaultConfig;

    ConfigManager (){};

    template <typename T>
    void ReadValue (const std::string &tableName, const std::string &key,
                    T &out, bool tmp = false);

    template <typename T>
    void AddDebugInput (const std::string &tableName, const std::string &key,
                        T &value);

    template <typename T>
    void AddDebugInput (const std::string &tableName, const std::string &key,
                        T &value, void (*func) (T *));

    bool GetIsEnabled (const std::string &name);

    template <typename T>
    void
    ProcessConfigOption (const std::string &tableName, const std::string &key,
                         T &out)
    {
        if constexpr (HasCustomDebugInput<T>)
            {
                AddDebugInput (tableName, key, out.Get (), &T::DebugInput);
                ReadValue (tableName, key, out.Get ());
            }
        else
            {
                AddDebugInput (tableName, key, out);
                ReadValue (tableName, key, out);
            }
    }

public:
    /// Returns the static instance for ConfigManager.
    static ConfigManager *GetInstance ();

    /// Initialises
    ConfigManager (const std::string &file = "config.toml");

    template <typename T>
    static void
    GetValue (const std::string &table, const std::string &key, T &out)
    {
        return GetInstance ()->ReadValue (table, key, out);
    }

    template <typename... Args>
    static bool
    ReadConfig (const std::string &table, Args... params)
    {
        if (!GetInstance ()->GetIsEnabled (table))
            return false;

        (GetInstance ()->ProcessConfigOption (table, params.first, *params.second), ...);
        return true;
    }
};
