#include <texture.h>
#include <interval.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
CheckerTexture::CheckerTexture( double scale,
                                const Color &c1,
                                const Color &c2 )
    : CheckerTexture( scale,
                      std::make_shared<SolidColor>( c1 ),
                      std::make_shared<SolidColor>( c2 ) )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
Color CheckerTexture::Value( double u, double v, const Point3 &p ) const
{
    auto xInteger = int( std::floor( _invScale * p.x() ) );
    auto yInteger = int( std::floor( _invScale * p.y() ) );
    auto zInteger = int( std::floor( _invScale * p.z() ) );

    bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

    return isEven ? _even->Value( u, v, p ) : _odd->Value( u, v, p );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
Color ImageTexture::Value( double u, double v, const Point3 &p ) const
{
    // If we have no texture data, then return solid cyan as a debugging aid.
    if ( _image.Height() <= 0 )
        return Color( 0, 1, 1 );

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = Interval( 0, 1 ).Clamp( u );
    v = 1.0 - Interval( 0, 1 ).Clamp( v );  // Flip V to image coordinates

    auto i = int( u * _image.Width() );
    auto j = int( v * _image.Height() );
    auto pixel = _image.PixelData( i, j );

    auto colorScale = 1.0 / 255.0;
    return Color( colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2] );
}
