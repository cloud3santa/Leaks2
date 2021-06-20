#pragma once
#include "COption.h"
#include "xorstring.hpp"

class CBoolOption : public COption
{
    std::string left_;
    std::string desc_;
    std::function< void( ) > func_;
    bool * bool_;
public:
    CBoolOption( std::string left, bool * bol, std::string desc = "",
                 std::function< void( ) > func = []
                 {
                 } )
        :
        left_( std::move( left ) ),
        desc_( std::move( desc ) ),
        func_( std::move( func ) ),
        bool_( std::move( bol ) )
    {
    }

    CBoolOption( const CBoolOption & obj ) = default;
    CBoolOption & operator=( const CBoolOption & obj ) = default;
    CBoolOption( CBoolOption && obj ) = default;
    CBoolOption & operator=( CBoolOption && obj ) = default;
    ~CBoolOption( ) = default;

    std::string left_text( ) const override
    {
        return left_;
    }

    std::string right_text( ) const override
    {
        return "";
    }

    std::string description( ) const override
    {
        return desc_;
    }

    void action( ) override
    {
        if ( keys_map_[k_press] . pressed )
        {
            *bool_ ^= true;
            func_( );
        }
    }

    sprite_info sprite( ) const override
    {
        if ( *bool_ )
            return { "mprankbadge", "globe_bg", 0, 255, 0, 255 };
        return { "mprankbadge", "globe_bg", 255, 0, 0, 255 };
    }

    std::string get_type( ) const override
    {
        return ( "CBoolOption" );
    }
};
