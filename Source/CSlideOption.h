#pragma once
#include "COption.h"
#include <iomanip>

template < typename T >
class CSlideOption : public COption
{
    std::string left_;
    std::string desc_;
    std::function< void( ) > func_;
    T * var_;
    T min_;
    T max_;
    T step_;
public:
    CSlideOption( std::string left, T * var, T min, T max, T step, std::string desc = "",
                  std::function< void( ) > func = []
                  {
                  } )
        :
        left_( std::move( left ) ),
        desc_( std::move( desc ) ),
        func_( std::move( func ) ),
        var_( std::move( var ) ),
        min_( std::move( min ) ),
        max_( std::move( max ) ),
        step_( std::move( step ) )
    {
    }

    CSlideOption( const CSlideOption & obj ) = default;
    CSlideOption & operator=( const CSlideOption & obj ) = default;
    CSlideOption( CSlideOption && obj ) = default;
    CSlideOption & operator=( CSlideOption && obj ) = default;
    ~CSlideOption( ) = default;

    std::string left_text( ) const override
    {
        return left_;
    }

    std::string right_text( ) const override
    {
        std::stringstream ss;
        ss << std::setprecision( 4 ) << *var_;
        std::stringstream ss2;
        ss2 << max_;
        return "< " + ss . str( ) + " > / " + ss2 . str( );
    }

    std::string description( ) const override
    {
        return desc_;
    }

    void action( ) override
    {
        if ( keys_map_[k_press] . pressed )
            func_( );
        if ( keys_map_[k_right] . pressed )
        {
            if ( *var_ <= max_ - step_ )
                *var_ += step_;
            else if ( *var_ < max_ )
                *var_ = max_;
            else
                *var_ = min_;
        }
        if ( keys_map_[k_left] . pressed )
        {
            if ( *var_ > min_ + step_ + step_ / 10 )
                *var_ -= step_;
            else if ( *var_ > min_ )
                *var_ = min_;
            else
                *var_ = max_;
        }
    }

    sprite_info sprite( ) const override
    {
        return { "", "", 0, 0, 0, 0 };
    }

    std::string get_type( ) const override
    {
        return XS("CSlideOption");
    }
};
