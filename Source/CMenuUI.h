#pragma once
#include "script.h"
#include "COption.h"

typedef struct
{
    short vk_key;
    bool pressed;
} keyinfo;

typedef struct
{
    int menutab;
    bool selected;
} menustruct;

typedef struct
{
    std::string menustring;
    std::string uiname;
    std::string icon_name;
    int tab_order;
} tab;

enum keys
{
    k_up,
    k_down,
    k_left,
    k_right,
    k_menuback,
    k_menuforward,
    k_open,
    k_close,
    k_press,
    k_max_key
};

extern std::map< int , keyinfo > keys_map_;

class CMenuUI
{
    /**
    * \brief This string identifies the currently chosen menu.
    */
    std::string menu_string_ = "player_options";
    tab current_tab_ = { "player_options", "Player", "icon_user", 0 };
    std::string submenu_string_ = "main";
    /**
    * \brief These variables declare where the menu will be drawn on the screen.
    */
    float menux_ = 0.7f;
    float menuy_ = 0.2f;

    /**
    * \brief The following variables dictate menu SIZE. (width, height)
    */
    float menuwidth_ = 0.25f;
    float menuheight_ = 0.5f;


    /**
    * \brief Holds status of menu being open.
    */
    bool opened_ = false;

    int controltimer = GetTickCount( );
    int giftimer_ = GetTickCount( );
    int gif_frame_ = 0;
    int current_option_ = 0;
    float previous_scroller_float_ = 0;
    int prev_option_ = 0;
    std::string frame_name_ = "";

    std::vector< std::string > previous_subs_;
    std::vector< COption* > current_options_;

public:
    /**
    * \brief A whole lotta constructors shenanigans
    */
    CMenuUI( );
    CMenuUI( const CMenuUI & obj ) = default;
    CMenuUI & operator=( const CMenuUI & obj ) = default;
    CMenuUI( CMenuUI && obj ) = default;
    CMenuUI & operator=( CMenuUI && obj ) = default;
    ~CMenuUI( ) = default;
    std::string texture_name( ) const { return frame_name_; };
    int get_frame( ) const { return gif_frame_; };
    void setup_key( int key, short vk_key );
    void setup_keys( );
    /**
    * \brief Check the keys. Run every tick.
    */
    void check_keys( );
    void navigate( );
    static float get_text_width( const std::string & text, int, float );
    float right_align_text_coord( const std::string & text ) const;
    void add_menu( const std::string & menustring, const std::string & uiname, const std::string & icon_name,
                   const std::function< void( ) >
                   & function ) const;
    void add_sub( const std::string & menustring, const std::string & title,
                  const std::function< void( ) > & function ) const;
    void draw_scroller( int * prev, int );
    void set_sub( const std::string & sub );;

    template < typename T >
    void push_option( T option )
    {
        current_options_ . push_back( option );
    }
    /**
    * \brief render the menu. everything drawing related will go through here.
    */
    void render( );
    std::map< int , keyinfo > get_keys_map( ) const { return keys_map_; };

    /**
    * \brief used to retrieve private values. aka getters
    * \return var value.
    */
    float menux( ) const;
    float menuy( ) const { return menuy_; }
    float menuwidth( ) const { return menuwidth_; }
    float menuheight( ) const { return menuheight_; }
    bool opened( ) const { return opened_; };
    tab current_tab() const { return current_tab_; }
};
