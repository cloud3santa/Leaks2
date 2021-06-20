#include "stdafx.h"
#include "CMenuUI.h"
#include "CDrawManager.h"
#include "natives_lower.h"
std::map< int , keyinfo > keys_map_;
int tabs = 0;
std::vector< tab > tabs_ { };
bool first_start_ = true;

void CMenuUI::setup_key( const int key, short vk_key )
{
    keys_map_[key] = { vk_key, false };
}

void CMenuUI::setup_keys( )
{
    setup_key( k_open , VK_MULTIPLY );
    setup_key( k_down , VK_NUMPAD2 );
    setup_key( k_left , VK_NUMPAD4 );
    setup_key( k_menuback , VK_NUMPAD1 );
    setup_key( k_menuforward , VK_NUMPAD3 );
    setup_key( k_press , VK_NUMPAD5 );
    setup_key( k_up , VK_NUMPAD8 );
    setup_key( k_right , VK_NUMPAD6 );
    setup_key( k_max_key , VK_NUMPAD0 );
}

void CMenuUI::check_keys( )
{
    for ( auto i = 10 ; i-- ; )
        keys_map_[i] . pressed = false;
    if ( static_cast< int >( GetTickCount( ) ) - controltimer >= 123 )
    {
        for ( auto i = 10 ; i-- ; )
            keys_map_[i] . pressed = GetAsyncKeyState( keys_map_[i] . vk_key );
        controltimer = GetTickCount( ); // Reset the timer.
    }
    current_options_ . clear( );
    tabs_ . clear( );
}

std::map< int , std::string > menu_map_;

void CMenuUI::navigate( )
{
    if ( keys_map_[k_open] . pressed )
        opened_ ^= true;
    if ( keys_map_[k_max_key] . pressed )
    {
        if ( !previous_subs_ . empty( ) )
        {
            submenu_string_ = previous_subs_ . back( );
            previous_subs_ . pop_back( );
        }
        else
            opened_ ^= true;
    }
    if ( keys_map_[k_down] . pressed )
    {
        if ( current_option_ + 1 >= int( current_options_ . size( ) ) )
            current_option_ = 0;
        else
            current_option_++;
    }
    if ( keys_map_[k_up] . pressed )
    {
        if ( current_option_ - 1 == -1 )
            current_option_ = current_options_ . size( ) - 1;
        else
            current_option_--;
    }
    if ( keys_map_[k_menuforward] . pressed )
    {
        if ( current_tab_ . tab_order + 1 >= int( tabs_ . size( ) ) )
            current_tab_ = tabs_[0];
        else
            current_tab_ = tabs_[current_tab_ . tab_order + 1];

        submenu_string_ = "main";
        current_option_ = 0;
    }
    if ( keys_map_[k_menuback] . pressed )
    {
        if ( ( current_tab_ . tab_order - 1 ) == -1 )
            current_tab_ = tabs_[tabs_ . size( ) - 1];
        else
            current_tab_ = tabs_[current_tab_ . tab_order - 1];
        submenu_string_ = "main";
        current_option_ = 0;
    }
}

float CMenuUI::get_text_width( const std::string & text, const int font, const float size )
{
    ui::_begin_text_command_width( static_cast< char* >( "STRING" ) );
    ui::set_text_font( font );
    ui::set_text_scale( size , size );
    ui::add_text_component_substring_player_name( const_cast< char* >( text . c_str( ) ) );
    return ui::_end_text_command_get_width( true );
}

float CMenuUI::right_align_text_coord( const std::string & text ) const
{
    return menux_ + 0.235f / 2.f - get_text_width( text , 0 , 0.3f );
}

void CMenuUI::add_menu( const std::string & menustring, const std::string & uiname, const std::string & icon_name,
                        const std::function< void( ) > & function ) const
{
    const tab to_push = { menustring, uiname, icon_name, int( tabs_ . size( ) ) };
    tabs_ . push_back( to_push );
    if ( current_tab_ . menustring == menustring )
        function( );
}

void CMenuUI::add_sub( const std::string & menustring, const std::string & title,
                       const std::function< void( ) > & function ) const
{
    if ( submenu_string_ == menustring )
    {
        graphics::_set_2d_layer( 2 );
        CDrawing::string( title , { menux_ + 0.07f, menuy_ - 0.09f, 0.3f, 0.3f } , CColor( 255 , 255 , 255 , 255 ) , 7 ,
                          false , true , true );
        function( );
    }
}

void CMenuUI::draw_scroller( int * prev, const int cur )
{
    if ( previous_scroller_float_ == 0 )
        previous_scroller_float_ = *prev * 0.035f + ( menuy_ - 0.03185f );
    const auto curf = cur * 0.035f + ( menuy_ - 0.03185f );
    if ( *prev != cur )
    {
        if ( ( *prev == static_cast< int >( current_options_ . size( ) ) - 1 ) && cur == 0 )
        {
            previous_scroller_float_ = previous_scroller_float_ - 0.005f;
            CDrawing::rect( { menux_ + 0.0305f, previous_scroller_float_, 0.1885f, 0.037f } ,
                            CColor( 255 , 255 , 255 , 255 ) );
            if ( previous_scroller_float_ <= curf )
            {
                previous_scroller_float_ = curf;
                *prev = cur;
            }
        }
        if ( previous_scroller_float_ < curf )
        {
            if ( previous_scroller_float_ + 0.005f > curf )
                previous_scroller_float_ = previous_scroller_float_ + ( curf - previous_scroller_float_ );
            else
                previous_scroller_float_ = previous_scroller_float_ + 0.005f;
            CDrawing::rect( { menux_ + 0.0305f, previous_scroller_float_, 0.1885f, 0.037f } ,
                            CColor( 255 , 255 , 255 , 255 ) );
            if ( previous_scroller_float_ >= curf )
            {
                previous_scroller_float_ = curf;
                *prev = cur;
            }
        }
        if ( previous_scroller_float_ > curf )
        {
            if ( previous_scroller_float_ - 0.005f < curf )
                previous_scroller_float_ = previous_scroller_float_ - ( previous_scroller_float_ - curf );
            previous_scroller_float_ = previous_scroller_float_ - 0.005f;
            if ( previous_scroller_float_ <= curf )
            {
                previous_scroller_float_ = curf;
                *prev = cur;
            }
            CDrawing::rect( { menux_ + 0.0305f, previous_scroller_float_, 0.1885f, 0.037f } ,
                            CColor( 255 , 255 , 255 , 255 ) );
        }
    }
    else
    {
        *prev = cur;

        CDrawing::rect( { menux_ + 0.0305f, cur * 0.035f + ( menuy_ - 0.03185f ), 0.1885f, 0.037f } ,
                        CColor( 255 , 255 , 255 , 255 ) );
    }
}

void CMenuUI::set_sub( const std::string & sub )
{
    previous_subs_ . push_back( submenu_string_ );
    submenu_string_ = sub;
    current_option_ = 0;
}

void CMenuUI::render( )
{
    graphics::_set_2d_layer( 1 );

    CDrawing::rect( { menux_, menuy_ + 0.1f, menuwidth_, menuheight_ } , CColor( 46 , 46 , 46 , 235 ) ); // Backround
    CDrawing::rect( { menux_ - 0.065f, menuy_ + 0.1f, 0.0025f, menuheight_ } ,
                    CColor( 255 , 255 , 255 , 255 ) );// Submenu Divider
    CDrawing::rect( { menux_ + 0.03f, menuy_ - 0.052345f, menuwidth_ - 0.06f, 0.0035f } ,
                    CColor( 255 , 255 , 255 , 255 ) );// Title Divider
    graphics::_set_2d_layer( 3 );
    CDrawing::rect( { menux_, menuy_ + 0.353f, menuwidth_, 0.005f } , CColor( 255 , 255 , 255 , 255 ) );
    graphics::_set_2d_layer( 1 );
    CDrawing::rect( { menux_, menuy_ - 0.1525f, menuwidth_, 0.005f } , CColor( 255 , 255 , 255 , 255 ) );
    CDrawing::rect( { menux_ + 0.0305f, menuy_ + 0.326f, 0.1885f, 0.045f } , { 35, 35, 35, 255 } );
    CDrawing::sprite( "kkk1234" , "icon_info" , { menux_ - 0.0575f, menuy_ + 0.3345f, 0.0075f, 0.015f } ,
                      CColor( 255 , 255 , 255 , 255 ) , 0.f );
    if ( static_cast< int >( GetTickCount( ) ) - giftimer_ >= 15 )
    {
        std::stringstream lol;
        lol << gif_frame_;
        const auto frame = lol . str( );
        if ( gif_frame_ == 175 )
            gif_frame_ = 0;
        if ( gif_frame_ == 0 )
            frame_name_ = "Comp 1_00000";
        else if ( ( gif_frame_ > 0 ) && ( gif_frame_ < 10 ) )
            frame_name_ = "Comp 1_0000" + frame;
        else if ( ( gif_frame_ > 9 ) && ( gif_frame_ < 100 ) )
            frame_name_ = "Comp 1_000" + frame;
        else if ( ( gif_frame_ > 99 ) && ( gif_frame_ < 1000 ) )
            frame_name_ = "Comp 1_00" + frame;
        gif_frame_++;
        giftimer_ = GetTickCount( );
    }

    CDrawing::sprite( "kkk1234" , frame_name_ , { menux_ + 0.03f, menuy_ - 0.1f, 0.1885f, 0.0765f } ,
                      CColor( 255 , 255 , 255 , 255 ) , 0.0f ); // Moddersonly Logo

    auto start_point = 0;
    auto end_point = current_options_ . size( ) > 10 ? 10 : current_options_ . size( ); // Max Options = 10;
    if ( current_options_ . size( ) > 10 && current_option_ >= 10 )
    {
        start_point = current_option_ - 10 + 1; // current_option_ - 15
        end_point = current_option_ + 1;
    }

    for ( auto i = start_point, j = 0 ; i < static_cast< int >( end_point ) ; ++i, ++j )
    {
        if ( current_option_ == i )
        {
            draw_scroller( &prev_option_ , j );
            graphics::_set_2d_layer( 2 );
            CDrawing::string( current_options_[i] -> left_text( ) ,
                              { menux_ - 0.057f, j * 0.035f + ( menuy_ - 0.0425f ), 0.3f, 0.3f } ,
                              CColor( 0 , 0 , 0 , 255 ) , 0 , false , false , false );
            CDrawing::string( current_options_[i] -> right_text( ) ,
                              {
                                  right_align_text_coord( current_options_[i] -> right_text( ) ),
                                  j * 0.035f + ( menuy_ - 0.0425f ), 0.3f, 0.3f
                              } , CColor( 0 , 0 , 0 , 255 ) , 0 , false , false , false );
            CDrawing::sprite( current_options_[i] -> sprite( ) . dict , current_options_[i] -> sprite( ) . name ,
                              { menux_ + 0.1125f, j * 0.035f + ( menuy_ - 0.03f ), 0.00675f, 0.0135f } ,
                              CColor( current_options_[i] -> sprite( ) . r , current_options_[i] -> sprite( ) . g ,
                                      current_options_[i] -> sprite( ) . b , current_options_[i] -> sprite( ) . a ) ,
                              0.f );
            graphics::_set_2d_layer( 1 );
            current_options_[i] -> action( );

            if ( !current_options_[i] -> description( ) . empty( ) )
                CDrawing::string( current_options_[i] -> description( ) ,
                                  { menux_ - 0.052525f, menuy_ + 0.32f, 0.275f, 0.275f } ,
                                  CColor( 255 , 255 , 255 , 255 ) , 0 , false , false , false );
            else
                CDrawing::string( "No description." , { menux_ - 0.052525f, menuy_ + 0.32f, 0.275f, 0.275f } ,
                                  CColor( 255 , 255 , 255 , 255 ) , 0 , false , false , false );
            CDrawing::string(
                std::to_string( current_option_ + 1 ) + "/" + std::to_string( current_options_ . size( ) ) , {
                    right_align_text_coord(
                        std::to_string( current_option_ + 1 ) + "/" + std::to_string( current_options_ . size( ) ) ) +
                    0.0075f,
                    menuy_ + 0.32f, 0.275f, 0.275f
                } , CColor( 255 , 255 , 255 , 255 ) , 0 , false , false , false );
        }
        else
        {
            graphics::_set_2d_layer( 2 );
            CDrawing::string( current_options_[i] -> left_text( ) ,
                              { menux_ - 0.057f, j * 0.035f + ( menuy_ - 0.0425f ), 0.3f, 0.3f } ,
                              CColor( 255 , 255 , 255 , 255 ) , 0 , false , false , false );
            CDrawing::string( current_options_[i] -> right_text( ) ,
                              {
                                  right_align_text_coord( current_options_[i] -> right_text( ) ),
                                  j * 0.035f + ( menuy_ - 0.0425f ), 0.3f, 0.3f
                              } , CColor( 255 , 255 , 255 , 255 ) , 0 , false , false , false );
            CDrawing::sprite( current_options_[i] -> sprite( ) . dict , current_options_[i] -> sprite( ) . name ,
                              { menux_ + 0.1125f, j * 0.035f + ( menuy_ - 0.03f ), 0.00675f, 0.0135f } ,
                              CColor( current_options_[i] -> sprite( ) . r , current_options_[i] -> sprite( ) . g ,
                                      current_options_[i] -> sprite( ) . b , current_options_[i] -> sprite( ) . a ) ,
                              0.f );
            graphics::_set_2d_layer( 1 );
        }
    }


    auto start_point2 = 0;
    auto end_point2 = tabs_ . size( ) > 5 ? 5 : tabs_ . size( ); // Max Options = 10;
    if ( tabs_ . size( ) > 5 && current_tab_ . tab_order >= 5 )
    {
        start_point2 = current_tab_ . tab_order - 5 + 1; // current_option_ - 15
        end_point2 = current_tab_ . tab_order + 1;
    }

    for ( auto i = start_point2, j = 0 ; i < static_cast< int >( end_point2 ) ; ++i, ++j )
    {
        if ( current_tab_ . tab_order == i )
        {
            CDrawing::rect( { menux_ - 0.0955f, ( j + 1 ) * 0.1f + ( menuy_ - 0.2f ), 0.0585f, 0.1f } ,
                            CColor( 255 , 255 , 255 , 255 ) );
            CDrawing::sprite( "kkk1234" , current_tab_ . icon_name ,
                              { menux_ - 0.0955f, ( j + 1 ) * 0.1f + ( menuy_ - 0.2005f ), 0.025f, 0.0435f } ,
                              CColor( 0 , 0 , 0 , 255 ) , 0.f );
            CDrawing::string( tabs_[i] . uiname ,
                              { menux_ - 0.0955f, ( j + 1 ) * 0.1f + ( menuy_ - 0.18f ), 0.3f, 0.3f } ,
                              CColor( 0 , 0 , 0 , 255 ) , 0 , false , false , true );
        }
        else
        {
            CDrawing::rect( { menux_ - 0.0955f, ( j + 1 ) * 0.1f + ( menuy_ - 0.2f ), 0.0585f, 0.1f } ,
                            CColor( 35 , 35 , 35 , 255 ) );
            CDrawing::sprite( "kkk1234" , tabs_[i] . icon_name ,
                              { menux_ - 0.0955f, ( j + 1 ) * 0.1f + ( menuy_ - 0.2f ), 0.025f, 0.0435f } ,
                              CColor( 255 , 255 , 255 , 255 ) , 0.f );
            CDrawing::string( tabs_[i] . uiname ,
                              { menux_ - 0.0955f, ( j + 1 ) * 0.1f + ( menuy_ - 0.18f ), 0.3f, 0.3f } ,
                              CColor( 255 , 255 , 255 , 255 ) , 0 , false , false , true );
        }
    }
}

float CMenuUI::menux( ) const
{
    return menux_;
}

CMenuUI::CMenuUI( )
= default;
