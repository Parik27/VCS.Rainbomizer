#include "base.hh"
#include "imgui.h"
#include "vcs/CPlayer.hh"
#include "vcs/CRunningScript.hh"
#include <vendor/imgui_memory_editor.h>

#include <array>

class ScriptsDebugInterface : public DebugInterface
{
    inline static std::array GlobalVariables = {
        std::make_tuple ("PLAYER_CHAR", 782, ImGuiDataType_S32),
        std::make_tuple ("ONMISSION", 789, ImGuiDataType_S32),
        std::make_tuple ("developer_flag", 2, ImGuiDataType_S32),
        std::make_tuple ("autostart_firstmission_flag", 3, ImGuiDataType_S32),
        std::make_tuple ("saveload_inprogress_flag", 4, ImGuiDataType_S32),
        std::make_tuple ("save_allowed_flag???", 5, ImGuiDataType_S32),
        std::make_tuple ("missionchain_upcoming_missions", 6,
                         ImGuiDataType_S32),
        std::make_tuple ("missionchain_completed_flags", 18, ImGuiDataType_S32),
        std::make_tuple ("mission_unlocks", 30, ImGuiDataType_S32),
        std::make_tuple ("acts_completed", 130, ImGuiDataType_S32),
        std::make_tuple ("stored_player_weapons???", 244, ImGuiDataType_S32),
        std::make_tuple ("stored_player_ammunitions", 254, ImGuiDataType_S32),
        std::make_tuple ("stored_player_weapon_models???", 264,
                         ImGuiDataType_S32),
        std::make_tuple ("stored_player_weapon_selected", 274,
                         ImGuiDataType_S32),
        std::make_tuple ("stored_player_armor_amount", 275, ImGuiDataType_S32),
        std::make_tuple ("stored_player_money_amount", 276, ImGuiDataType_S32),
        std::make_tuple ("something related to current safehouse", 277,
                         ImGuiDataType_S32),
        std::make_tuple ("savelocation_safehouse_index???", 281,
                         ImGuiDataType_S32),
        std::make_tuple ("savelocation_asset_index???", 282, ImGuiDataType_S32),
        std::make_tuple ("saved_player_cameramode_onfoot", 283,
                         ImGuiDataType_S32),
        std::make_tuple (
            "something related to respawn X coordinate after loading", 284,
            ImGuiDataType_S32),
        std::make_tuple (
            "something related to respawn Y coordinate after loading", 285,
            ImGuiDataType_S32),
        std::make_tuple (
            "something related to respawn Z coordinate after loading", 286,
            ImGuiDataType_S32),
        std::make_tuple ("something related to respawn angle after loading",
                         287, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_busted_flag", 288, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_wasted_flag", 289, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_taxiwarp_flag", 290,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_weaponbuyback_flag", 291,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_health_flag", 292, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_police_flag", 293, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_policeflash_flag", 294,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_armor_flag", 295, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_sprint_flag", 296, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_waterclimb_flag", 297,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_camera_flag", 298, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_policebribe_flag", 299,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_inventory_flag", 300,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_throwable_flag", 301,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_radio_flag", 302, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_bike_flag", 303, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_driveby_flag", 304, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_drivebybike_flag", 305,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_freecamera_flag", 306,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_rhino_flag", 307, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_sprayshop_flag", 308,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_sirens_flag", 309, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_metaldetector_flag", 310,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_bombshop_flag", 311,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_golf_flag", 312, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_shootingrange_flag", 313,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_jerry_flag", 314, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_carlook_flag", 315, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_carcamera_flag", 316,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_boat_flag", 317, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_helicopter_flag", 318,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_minigun_flag", 319, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_hunter_flag", 320, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_autogyro_flag", 321,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_sesparow_flag", 322,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_export_flag", 323, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_exportreward_flag", 324,
                         ImGuiDataType_S32),
        std::make_tuple ("something related to empire site status help message",
                         325, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_stonewallj_flag", 326,
                         ImGuiDataType_S32),
        std::make_tuple ("empire_reputation_allmax_flag", 327,
                         ImGuiDataType_S32),
        std::make_tuple ("multiplayer_content_unlocks", 328, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_race_flag", 332, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_jetskitrial_flag", 333,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_meleeattack_flag", 334,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_meleeblock_flag", 335,
                         ImGuiDataType_S32),
        std::make_tuple ("help_displayed_meleegrab_flag", 336,
                         ImGuiDataType_S32),
        std::make_tuple ("empire_owned_amount30_flag", 337, ImGuiDataType_S32),
        std::make_tuple ("help_displayed_taxireward_flag", 338,
                         ImGuiDataType_S32),
        std::make_tuple ("empire_owned_amount5_flag", 339, ImGuiDataType_S32),
        std::make_tuple ("empire_owned_amount10_flag", 340, ImGuiDataType_S32),
        std::make_tuple ("empire_owned_amount15_flag", 341, ImGuiDataType_S32),
        std::make_tuple ("empire_owned_amount20_flag", 342, ImGuiDataType_S32),
        std::make_tuple ("empire_owned_amount25_flag", 343, ImGuiDataType_S32),
        std::make_tuple ("clothes_locked_flags", 344, ImGuiDataType_S32),
        std::make_tuple ("clothes_unlock_inprogress_plr_flag", 358,
                         ImGuiDataType_S32),
        std::make_tuple ("clothes_unlock_inprogress_plr2_flag", 359,
                         ImGuiDataType_S32),
        std::make_tuple ("clothes_unlock_inprogress_plr3_flag", 360,
                         ImGuiDataType_S32),
        std::make_tuple ("clothes_unlock_inprogress_plr4_flag", 361,
                         ImGuiDataType_S32),
        std::make_tuple ("clothes_unlock_inprogress_plr5_flag", 362,
                         ImGuiDataType_S32),
        std::make_tuple ("clothes_unlock_inprogress_plr6_flag", 363,
                         ImGuiDataType_S32),
        std::make_tuple ("clothes_unlock_inprogress_plr7_flag", 364,
                         ImGuiDataType_S32),
        std::make_tuple ("clothes_unlock_inprogress_plr8_flag", 365,
                         ImGuiDataType_S32),
        std::make_tuple ("clothes_unlock_inprogress_plr9_flag", 366,
                         ImGuiDataType_S32),
        std::make_tuple ("clothes_unlock_inprogress_plr10_flag", 367,
                         ImGuiDataType_S32),
        std::make_tuple ("clothes_unlock_inprogress_plr11_flag", 368,
                         ImGuiDataType_S32),
        std::make_tuple ("clothes_unlock_inprogress_plr12_flag", 369,
                         ImGuiDataType_S32),
        std::make_tuple ("clothes_unlock_inprogress_plr13_flag", 370,
                         ImGuiDataType_S32),
        std::make_tuple ("clothes_unlock_inprogress_plr14_flag", 371,
                         ImGuiDataType_S32),
        std::make_tuple ("clothes_index", 372, ImGuiDataType_S32),
        std::make_tuple ("empire_owner_gangs", 373, ImGuiDataType_S32),
        std::make_tuple ("empire_owner_gang_densities", 403, ImGuiDataType_S32),
        std::make_tuple ("empire_types", 433, ImGuiDataType_S32),
        std::make_tuple ("empire_sizes", 463, ImGuiDataType_S32),
        std::make_tuple ("empire_conditions", 493, ImGuiDataType_S32),
        std::make_tuple ("empire_states???", 523, ImGuiDataType_S32),

    };

    inline static constexpr std::array DataTypes
        = {std::make_pair ("int", ImGuiDataType_S32),
           std::make_pair ("uint", ImGuiDataType_U32),
           std::make_pair ("float", ImGuiDataType_Float)};

    void
    DrawGlobalsEditor ()
    {
        ImGui::Columns (3);
        for (auto &[name, idx, dataType] : GlobalVariables)
            {
                ImGui::PushID (idx);
                ImGui::InputScalar ("##data", dataType,
                                    &CTheScripts::GetGlobal<int> (idx));
                ImGui::NextColumn ();
		ImGui::Text ("%s", name);
		ImGui::NextColumn ();
                if (ImGui::BeginCombo ("Type", "int"))
                    {
                        for (auto &[name, type] : DataTypes)
                            {
                                if (ImGui::Selectable (name))
                                    {
                                        dataType = type;
                                        break;
                                    }
                            }
                        ImGui::EndCombo ();
                    }
                ImGui::NextColumn ();
                ImGui::PopID ();
            }
        ImGui::Columns (1);
    }

    void
    DrawRunningScripts ()
    {
      ImGui::BeginTable ("Running Scripts", 4);
      ImGui::TableSetupColumn ("Index");
      ImGui::TableSetupColumn ("Name");
      ImGui::TableSetupColumn ("IP");
      ImGui::TableSetupColumn ("Stack Pointer");

      ImGui::EndTable ();
    }

    void
    Draw () override
    {
        if (ImGui::CollapsingHeader ("Global Variables"))
            DrawGlobalsEditor ();

	if (ImGui::CollapsingHeader ("Running Scripts"))
	  DrawRunningScripts ();
    }

public:
    const char *
    GetName () const override
    {
        return "Script Debug Interface";
    }
} g_ScriptsDebugInterface;
