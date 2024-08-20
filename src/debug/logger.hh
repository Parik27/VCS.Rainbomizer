#pragma once

#include "base.hh"
#include "imgui.h"
#include <string_view>

class LoggerDebugInterface : public DebugInterface
{
    ImGuiTextBuffer             buf;
    static LoggerDebugInterface sm_Instance;

    void
    Draw () override
    {
        static bool bAutoscroll;

        if (ImGui::Button ("Clear"))
            buf.clear ();

        ImGui::Checkbox ("Autoscroll", &bAutoscroll);

        ImGui::TextUnformatted (buf.begin (), buf.end ());

        if (bAutoscroll && ImGui::GetScrollY () >= ImGui::GetScrollMaxY ())
            ImGui::SetScrollHereY (1.0f);
    }

public:
    template <typename... Args>
    static void
    PublishLogMessage (const char *format, va_list arg)
    {
        sm_Instance.buf.appendfv (format, arg);
        sm_Instance.buf.appendf ("\n");
    }

    const char *
    GetName () const override
    {
        return "Log";
    }
};

inline LoggerDebugInterface LoggerDebugInterface::sm_Instance{};
