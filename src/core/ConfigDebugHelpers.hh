#ifdef ENABLE_DEBUG_MENU
#include <imgui.h>
#endif

#include <utility>

template <typename ValueType, const auto& ValueList>
class EnumConfigOption
{
    ValueType value;

public:
    static const char *
    GetName (ValueType *value)
    {
        for (auto &[name, val] : ValueList)
            {
                if (val == *value)
                    return name;
            }

        return "None";
    }

    static void
    DebugInput (ValueType *value)
    {
#ifdef ENABLE_DEBUG_MENU
        if (ImGui::BeginCombo ("", GetName (value)))
            {
                static ImGuiTextFilter filter;
                filter.Draw ("##filter");

                for (auto &[name, val] : ValueList)
                    {
                        if (filter.PassFilter (name)
                            && ImGui::Selectable (name))
                            {
                                *value = val;
                                filter.Clear ();
                            }
                    }
                ImGui::EndCombo ();
            }
#endif
    }

    ValueType &
    Get ()
    {
        return value;
    }

    EnumConfigOption (ValueType value) : value (value) {}

    operator ValueType () const { return value; }

    using value_type = ValueType;
};
