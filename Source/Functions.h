#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "natives_lower.h"

inline Hash $(const std::string &str)
{
    const auto len = str.size();
    unsigned int hash, i;
    for (hash = i = 0; i < len; ++i)
    {
        hash += tolower(str[i]);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

inline void rcoe( const Entity ent )
{
    const auto netid = network::network_get_network_id_from_entity(ent);
    auto tick = 0;

    while (!network::network_has_control_of_network_id(netid) && tick <= 69)
    {
        network::network_request_control_of_network_id(netid);
        tick++;
    }
}

inline void draw_icon_notif( const std::string & content, const std::string  & title, const std::string & subtitle, const std::string
                             & tag)
{
    auto clan = "___" + tag;
    ui::_set_notification_background_color(180);
    ui::_set_notification_text_entry("STRING");
    ui::add_text_component_substring_player_name(const_cast< char* >( content . c_str( ) ));
    ui::_set_notification_message_clan_tag_2( "CHAR_MP_FM_CONTACT" , "CHAR_MP_FM_CONTACT" , 1 , 7 ,
                                              const_cast< char* >( title . c_str( ) ) ,
                                              const_cast< char* >( subtitle . c_str( ) ) , 1 ,
                                              const_cast< char* >( clan . c_str( ) ) , 8 , 0 );
    ui::_draw_notification(0, 0);
}
#endif // FUNCTIONS_H
