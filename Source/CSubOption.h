#ifndef CSUBOPTION_H
#define CSUBOPTION_H
#include "script.h"
#include "COption.h"

class CSubOption : public COption
{
    std::string left_;
    std::string menu_;
    std::string desc_;
    std::function< void( ) > func_;
public:
    CSubOption( std::string left, std::string menu, std::string desc = "", std::function< void( ) > func = []
                {
                } )
        :
        left_( std::move( left ) ),
        menu_( std::move( menu ) ),
        desc_( std::move( desc ) ),
        func_( std::move( func ) )
    {
    }

    CSubOption( const CSubOption & obj ) = default;
    CSubOption & operator=( const CSubOption & obj ) = default;
    CSubOption( CSubOption && obj ) = default;
    CSubOption & operator=( CSubOption && obj ) = default;
    ~CSubOption( ) = default;

    std::string left_text( ) const override
    {
        return left_;
    }

    std::string right_text( ) const override
    {
        return ( "=>" );
    }

    std::string description( ) const override
    {
        return desc_;
    }

    sprite_info sprite( ) const override
    {
        return { "", "", 0, 0, 0, 0 };
    }

    void action( ) override
    {
        if ( keys_map_[k_press] . pressed )
        {
            func_( );
            set_menu_sub( menu_ );
        }
    }

    std::string get_type( ) const override
    {
        return ( "CSubOption" );
    }
};
#endif // CSUBOPTION_H
