#pragma once

#include <memory>
#include <color.h>
#include <perlin.h>
#include <rtSTBImage.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Texture
{
public:

    virtual ~Texture() = default;

    virtual Color Value( double u, double v, const Point3 &p ) const = 0;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class SolidColor : public Texture
{
public:

    SolidColor( const Color &albedo ) : _albedo( albedo ) {}

    SolidColor( double red, double green, double blue )
        : SolidColor( Color( red, blue, green ) )
    {}

    virtual Color Value( double u, double v, const Point3 &p ) const override
    {
        return _albedo;
    }

private:

    Color _albedo;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class CheckerTexture : public Texture
{
public:

    CheckerTexture( double scale,
                    const std::shared_ptr<Texture> &even,
                    const std::shared_ptr<Texture> &odd )
        : _invScale( 1.0 / scale ),
          _even( even ),
          _odd( odd )
    {}

    CheckerTexture( double scale,
                    const Color &c1,
                    const Color &c2 );

    Color Value( double u, double v, const Point3 &p ) const override;

private:

    double                   _invScale = 1.0;
    std::shared_ptr<Texture> _even;
    std::shared_ptr<Texture> _odd;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class ImageTexture : public Texture
{
public:

    ImageTexture( const std::filesystem::path &imageFile ) : _image( imageFile ) {}

    virtual Color Value( double u, double v, const Point3 &p ) const override;

private:

    rtImage _image;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class NoiseTexture : public Texture
{
public:

    NoiseTexture( double scale ) : _scale( scale ) {}

    virtual Color Value( double u, double v, const Point3 &p ) const override;

private:

    Perlin _noise;
    double _scale = 1.0;
};
