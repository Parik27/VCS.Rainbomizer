#include "base.hh"
#include "imgui.h"
#include "vcs/CPed.hh"
#include "vcs/CPlayer.hh"

#include <array>

class PlayerWeaponDebugInterface : public DebugInterface
{
    inline static std::array WeaponNames =
    {
        "WEAPON_UNARMED",
        "WEAPON_BRASSKNUCKLE",
        "WEAPON_BUTTERFLY",
        "WEAPON_GOLFCLUB",
        "WEAPON_NIGHTSTICK",
        "WEAPON_KNIFE",
        "WEAPON_BASEBALLBAT",
        "WEAPON_AXE",
        "WEAPON_GAFFHOOK",
        "WEAPON_MACHETE",
        "WEAPON_KATANA",
        "WEAPON_CHAINSAW",
        "WEAPON_GRENADE",
        "WEAPON_DETONATOR_GRENADE",
        "WEAPON_LANDMINE",
        "WEAPON_SEAMINE",
        "WEAPON_TEARGAS",
        "WEAPON_MOLOTOV",
        "WEAPON_ROCKET",
        "WEAPON_PISTOL",
        "WEAPON_PYTHON",
        "WEAPON_SHOTGUN",
        "WEAPON_SPAS12_SHOTGUN",
        "WEAPON_STUBBY_SHOTGUN",
        "WEAPON_SKORPION",
        "WEAPON_UZI",
        "WEAPON_SILENCED_INGRAM",
        "WEAPON_MP5",
        "WEAPON_M16",
        "WEAPON_AK47",
        "WEAPON_SNIPERRIFLE",
        "WEAPON_LASERSCOPE",
        "WEAPON_ROCKETLAUNCHER",
        "WEAPON_FLAMETHROWER",
        "WEAPON_M249",
        "WEAPON_MINIGUN",
        "WEAPON_DETONATOR",
        "WEAPON_HELICANNON",
        "WEAPON_CAMERA",
        "WEAPON_BINOCULARS"
    };

    void
    Draw () override
    {
        CPed *ped = FindPlayerPed ();
	if (!ped)
	  return;

	size_t idx = 0;
        ImGui::Columns (2);
        for (auto &weapon : ped->Weapons)
            {
                ImGui::PushID (idx);
                ImGui::Combo ("Weapon", (int *) &weapon.Type,
                              WeaponNames.data (), WeaponNames.size ());
		ImGui::NextColumn();
                ImGui::InputScalar ("Ammo", ImGuiDataType_U32, &weapon.Ammo);
		ImGui::NextColumn();
                ImGui::PopID ();
		idx++;
            }

	ImGui::Columns();
    }

public:
    const char *
    GetName () const override
    {
        return "Player Weapons";
    }
} g_PlayerWeaponDebugInterface;
