#pragma once

#include <string>
#include <variant>
#include <string_view>
#include <map>
#include "base.hh"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

class ConfigDebugInterface : public DebugInterface
{
public:
    using Type        = std::variant<int *, std::string *, bool *, double *>;

    struct ConfigGroup
    {
        std::map<std::string, Type> rawConfigOptions;
        std::map<std::string, std::pair<void (*) (void *), void*>>
            customizedConfigOptions;
    };

private:
    static auto &
    GetConfigOptions ()
    {
        static std::map<std::string, ConfigGroup> sm_ConfigGroups;
        return sm_ConfigGroups;
    }

    static ConfigDebugInterface sm_Instance;

    void
    DrawConfigGroup (ConfigGroup &group)
    {
        for (auto &i : group.rawConfigOptions)
            {
                ImGui::Columns (2);

                ImGui::Text ("%s", i.first.c_str ());
                ImGui::NextColumn ();

                ImGui::PushID (i.first.c_str ());
                std::visit (
                    [i] (auto &&arg) {
                        using T = std::decay_t<decltype (arg)>;

                        if constexpr (std::is_same_v<T, int *>)
                            ImGui::InputInt ("", arg);
                        else if constexpr (std::is_same_v<T, double *>)
                            ImGui::InputDouble ("", arg);
                        else if constexpr (std::is_same_v<T, std::string *>)
                            ImGui::InputText ("", arg);
                        else if constexpr (std::is_same_v<T, bool *>)
                            ImGui::Checkbox ("", arg);
                    },
                    i.second);
                ImGui::PopID ();
                ImGui::NextColumn ();

                ImGui::Columns ();
            }

        for (auto &[name, data] : group.customizedConfigOptions)
            {
                ImGui::Columns (2);
                ImGui::Text ("%s", name.c_str ());
                ImGui::NextColumn ();

                ImGui::PushID (name.c_str ());

                auto &[func, ptr] = data;
                func (ptr);
                ImGui::PopID();
                ImGui::NextColumn ();

                ImGui::Columns ();
            }
    }

    void
    Draw () override
    {
        for (auto &i : GetConfigOptions ())
            {
                if (ImGui::CollapsingHeader (i.first.c_str ()))
                    {
                        ImGui::PushID (i.first.c_str ());
                        DrawConfigGroup (i.second);
                        ImGui::PopID ();
                    }
            }
    }

public:
    template <typename T>
    static void
    AddConfigOption (const std::string &tableName, const std::string &key,
                     T *data)
    {
        GetConfigOptions ()[tableName].rawConfigOptions[key] = data;
    }

    template <typename T>
    static void
    AddConfigOption (const std::string &tableName, const std::string &key,
                     T *data, void (*func) (T *))
    {
        GetConfigOptions ()[tableName].customizedConfigOptions[key]
            = std::make_pair (reinterpret_cast<void (*) (void *)> (func), data);
    }

    const char *
    GetName () const override
    {
        return "Config";
    }
};

inline ConfigDebugInterface ConfigDebugInterface::sm_Instance{};
