#ifndef CREGOPTION_H
#define CREGOPTION_H
#include "script.h"
#include "COption.h"
#include "xorstring.hpp"

class CRegOption : public COption
{
    std::string left_;
    std::string right_;
    std::string desc_;
    std::function< void( ) > func_;
public:
    CRegOption( std::string left, std::string right, std::string desc = "", std::function< void( ) > func = []
                {
                } )
        :
        left_( std::move( left ) ),
        right_( std::move( right ) ),
        desc_( std::move( desc ) ),
        func_( std::move( func ) )
    {
    }

    CRegOption( const CRegOption & obj ) = default;
    CRegOption & operator=( const CRegOption & obj ) = default;
    CRegOption( CRegOption && obj ) = default;
    CRegOption & operator=( CRegOption && obj ) = default;
    ~CRegOption( ) = default;

    std::string left_text( ) const override
    {
        return left_;
    }

    std::string right_text( ) const override
    {
        return right_;
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
            func_( );
    }

    std::string get_type( ) const override
    {
        return ( "CRegOption" );
    }
};
#endif // CREGOPTION_H
