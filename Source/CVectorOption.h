#ifndef CVECTOROPTION_H
#define CVECTOROPTION_H
#include "script.h"
#include "COption.h"

template < class T >
class CVectorOption : public COption
{
    std::string left_;
    std::vector< T > vector_;
    std::string desc_;
    std::function< void( int ) > func_;
    int current_position_;
public:
    CVectorOption( std::string left, std::vector< T > vector, const int start = 0, std::string desc = "",
                   std::function< void( int ) > func = nullptr )
        :
        left_( std::move( left ) ),
        vector_( std::move( vector ) ),
        desc_( std::move( desc ) ),
        func_( std::move( func ) ),
        current_position_( start )
    {
    }

    CVectorOption( const CVectorOption & obj ) = default;
    CVectorOption & operator=( const CVectorOption & obj ) = default;
    CVectorOption( CVectorOption && obj ) = default;
    CVectorOption & operator=( CVectorOption && obj ) = default;
    ~CVectorOption( ) = default;

    std::string left_text( ) const override
    {
        return left_;
    }

    std::string right_text( ) const override
    {
        std::stringstream ss;
        ss << vector_ . at( current_position_ );
        const auto value = ss . str( );
        ss . clear( );
        return XS("< " + value + " > (" + std::to_string( current_position_ + 1 ) + "/" + std::to_string(
            vector_ . size( ) ) + ")");
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
            func_( current_position_ );

        if ( keys_map_[k_right] . pressed )
        {
            if ( current_position_ < static_cast< int >( vector_ . size( ) ) - 1 )
                current_position_ += 1;
            else
                current_position_ = 0;
        }
        if ( keys_map_[k_left] . pressed )
        {
            if ( current_position_ > 0 )
                current_position_ -= 1;
            else
                current_position_ = static_cast< int >( vector_ . size( ) ) - 1;
        }
    }

    std::string get_type( ) const override
    {
        return XS("CVectorOption");
    }
};
#endif // CVECTOROPTION_H
