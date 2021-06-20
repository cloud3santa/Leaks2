#pragma once
#include "script.h"

typedef struct
{
    float x, y, width, height;
} pos_size;

class CColor
{
    int r_;
    int g_;
    int b_;
    int a_;
public:
    CColor( int r, int g, int b, int a );
    CColor( float r, float g, float b, float a );
    int r( ) const;
    int g( ) const;
    int b( ) const;
    int a( ) const;
};

class CDrawing
{
public:
    static void rect( pos_size, CColor );
    static void string( const std::string & str, pos_size, CColor, int, bool, bool, bool );
    static void sprite( const std::string & dict, const std::string & texture, pos_size, CColor, float );
};
