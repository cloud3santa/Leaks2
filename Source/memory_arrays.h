#pragma once
#include "stdafx.h"
#include <vector>

namespace vars
{
    namespace mem_vectors
    {
        int WorldPTR_STEAM = 0x23D6330;
        int BlipPTR_STEAM = 0x1FC8850;

        //Player
        std::vector< DWORD > God_Mode { 0x08, 0x189 };
        std::vector< DWORD > Wanted_Level { 0x08, 0x10B8, 0x7F8 };
        std::vector< DWORD > Sprint_Speed { 0x08, 0x10B8, 0x14C };
        std::vector< DWORD > Swim_Speed { 0x08, 0x10B8, 0x0148 };
        std::vector< DWORD > Max_HP { 0x8, 0x2A0 };
        std::vector< DWORD > HP { 0x8, 0x280 };
        std::vector< DWORD > Armor { 0x8, 0x14B8 };
        std::vector< DWORD > No_Ragdoll { 0x8, 0x10A8 };
        std::vector< DWORD > Seatbelt { 0x8, 0x13EC };//on:201,off:200
        std::vector< DWORD > Stamina { 0x8, 0x10B8, 0xC60 };
        std::vector< DWORD > Damage_Multiplier { 0x8, 0x10B8, 0xC78 };
        std::vector< DWORD > Melee_Damage_Multiplier { 0x8, 0x10B8, 0xC80 };
        std::vector< DWORD > Vehicle_Damage_Multiplier { 0x08, 0x10B8, 0xC88 };
        std::vector< DWORD > InVehicle { 0x08, 0x146B };
        std::vector< DWORD > Fram_Eflags { 0x08, 0x10B8, 0x1F8 };

        //Weapon
        std::vector< DWORD > Spread { 0x8, 0x10C8, 0x20, 0x70 };
        std::vector< DWORD > Recoil { 0x8, 0x10C8, 0x20, 0x2A4 };
        std::vector< DWORD > Fast_Shoot { 0x8, 0x10C8, 0x20, 0x134 };
        std::vector< DWORD > BULLET_DMG { 0x8, 0x10C8, 0x20, 0xB0 };
        std::vector< DWORD > Reload_Multiplier { 0x8, 0x10C8, 0x20, 0x12C };
        std::vector< DWORD > Reload_Vehicle { 0x8, 0x10C8, 0x20, 0x128 };
        //Ammo
        std::vector< DWORD > Max_Ammo { 0x8, 0x10C8, 0x20, 0x60, 0x28 };
        std::vector< DWORD > Ammo_Type { 0x8, 0x10C8, 0x20, 0x60, 0x54 };
        std::vector< DWORD > current_Ammo { 0x8, 0x10C8, 0x20, 0x60, 0x8, 0x0, 0x18 };

        //Vehile
        std::vector< DWORD > God_Mode_Vehicle { 0x08, 0xD28, 0x189 };
        std::vector< DWORD > Vehicle_Health1 { 0x08, 0xD28, 0x280 };
        std::vector< DWORD > Vehicle_Health2 { 0x08, 0xD28, 0x89C };
        std::vector< DWORD > Vehicle_Boost { 0x08, 0xD28, 0x320 };
        std::vector< DWORD > Vehicle_DirtLevel { 0x08, 0xD28, 0x988 };
        std::vector< DWORD > Vehicle_Bullet_Proof_Tires { 0x08, 0xD28, 0x8D3 };
        std::vector< DWORD > Vehicle_Tires { 0x08, 0xD28, 0xB68 };
        std::vector< DWORD > Vehicle_Doors { 0x08, 0xD28, 0xB80 };
        std::vector< DWORD > Vehicle_ACCELERATION { 0x08, 0xD28, 0x8C8, 0x4C };
        std::vector< DWORD > Vehicle_BRAKEFORCE { 0x08, 0xD28, 0x8C8, 0x6C };
        std::vector< DWORD > Vehicle_TRACTION_CURVE_MIN { 0x08, 0xD28, 0x8C8, 0x90 };
        std::vector< DWORD > Vehicle_DEFORM_MULTIPLIER { 0x08, 0xD28, 0x8C8, 0xF8 };
        std::vector< DWORD > Vehicle_UPSHIFT { 0x08, 0xD28, 0x8C8, 0x58 };
        std::vector< DWORD > Vehicle_SUSPENSION_FORCE { 0x08, 0xD28, 0x8C8, 0xBC };
        std::vector< DWORD > Vehicle_Speed { 0x08, 0xD28, 0x790 };
        std::vector< DWORD > Vehicle_Gravity { 0x08, 0xD28, 0xBCC };


        //TP
        std::vector< DWORD > X { 0x8, 0x30, 0x50 };
        std::vector< DWORD > Y { 0x8, 0x30, 0x54 };
        std::vector< DWORD > Z { 0x8, 0x30, 0x58 };
    }
}
