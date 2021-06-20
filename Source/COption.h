#ifndef COPTION_H
#define COPTION_H
#include "CDrawManager.h"
#include "CMenuUI.h"

typedef struct
{
    std::string dict, name;
    int r, g, b, a;
} sprite_info;

class COption
{
public:
    COption( ) = default;
    COption( const COption & obj ) = default;
    COption & operator=( const COption & obj ) = default;
    COption( COption && obj ) = default;
    COption & operator=( COption && obj ) = default;
    virtual ~COption( ) = default;
    virtual void action( ) = 0;
    virtual std::string left_text( ) const = 0;
    virtual std::string right_text( ) const = 0;
    virtual std::string description( ) const = 0;
    virtual sprite_info sprite( ) const = 0;
    virtual std::string get_type( ) const = 0;
};

#endif // COPTION_H
