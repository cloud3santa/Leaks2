#pragma once
int selected_player = 0;
namespace vars
{

    namespace player
    {
        namespace ints
        {


            int wanted_level = 1;
            int p_opacity = 4.f;
            int v_boost_bind = 0x45; // E


        }
    }
    namespace vehicle
    {
        namespace ints
        {

            int v_boost_bind = 0x45; // E
            int v_jump_bind = 0x20; // Space
            int v_boost_speed = 90;
        }
    }
    namespace recovery
    {
        namespace ints
        {

            int r_stealth_delay = 100;
            int r_stealth_amount = 10000000;
        }
    }
}
