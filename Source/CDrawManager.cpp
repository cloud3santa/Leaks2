#include "stdafx.h"
#include "CDrawManager.h"
#include "script.h"
#include "natives_lower.h"


void CDrawing::rect( const pos_size posSize, const CColor rgba )
{
    graphics::draw_rect( posSize . x , posSize . y , posSize . width , posSize . height , rgba . r( ) , rgba . g( ) ,
                         rgba . b( ) , rgba . a( ) );
}


void CDrawing::string( const std::string & str, const pos_size possize,
                       const CColor rgba, const int font, const bool outline, const bool shadow,
                       const bool center )
{
    ui::set_text_font( font );
    ui::set_text_scale( possize . width , possize . height );
    ui::set_text_colour( rgba . r( ) , rgba . g( ) , rgba . b( ) , rgba . a( ) );
    if ( outline )
        ui::set_text_outline( );
    if ( shadow )
        ui::set_text_drop_shadow( );
    if ( center )
        ui::set_text_centre( 1 );
    ui::begin_text_command_display_text( static_cast< char* >( "STRING" ) );
    const auto drawing_text = str . c_str( );
    ui::add_text_component_substring_player_name( const_cast< char* >( drawing_text ) );
    ui::end_text_command_display_text( possize . x , possize . y );
}


void CDrawing::sprite( const std::string & dict, const std::string & texture, const pos_size possize,
                       const CColor rgba, const float rotation )
{
    if ( !graphics::has_streamed_texture_dict_loaded( const_cast< char* >( dict . c_str( ) ) ) )
    {
        graphics::request_streamed_texture_dict( const_cast< char* >( dict . c_str( ) ) , false );
    }
    else
    {
        graphics::draw_sprite( const_cast< char* >( dict . c_str( ) ) , const_cast< char* >( texture . c_str( ) ) ,
                               possize . x , possize . y , possize . width , possize . height , rotation , rgba . r( ) ,
                               rgba . g( ) , rgba . b( ) , rgba . a( ) );
    }
}

CColor::CColor( const int r, const int g, const int b, const int a ) : r_( r ), g_( g ), b_( b ), a_( a )
{
}

CColor::CColor( const float r, const float g, const float b, const float a ) : r_( r * 255 ), g_( g * 255 ),
                                                                               b_( b * 255 ), a_( a * 255 )
{
}

int CColor::r( ) const
{
    return r_;
}

int CColor::g( ) const
{
    return g_;
}

int CColor::b( ) const
{
    return b_;
}

int CColor::a( ) const
{
    return a_;
}
