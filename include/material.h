#pragma once

#include <memory>

#include <texture.h>
#include <hittable.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Material
{
public:

    virtual ~Material() = default;

    virtual bool Scatter( const Ray &r,
                          const HitRecord &rec,
                          Color &attenuation,
                          Ray &scattered ) const
    {
        return false;
    }

    virtual Color Emitted( double u, double v, const Point3 &p ) const
    {
        return Color( 0, 0, 0 );
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Lambertian : public Material
{
public:

    Lambertian( const Color &albedo ) : _texture( std::make_shared<SolidColor>( albedo ) ) {}
    Lambertian( const std::shared_ptr<Texture> &tex ) : _texture( tex ) {}

    virtual bool Scatter( const Ray &r,
                          const HitRecord &rec,
                          Color &attenuation,
                          Ray &scattered ) const override;

private:

    std::shared_ptr<Texture> _texture;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Metal : public Material
{
public:

    Metal( const Color &albedo, double fuzz )
        : _albedo( albedo ),
          _fuzz( fuzz < 1 ? fuzz : 1 )
    {}

    virtual bool Scatter( const Ray &r,
                          const HitRecord &rec,
                          Color &attenuation,
                          Ray &scattered ) const override;

private:

    Color  _albedo;
    double _fuzz = 0.0;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Dielectric : public Material
{
public:

    Dielectric(double refractionIndex)
        : _refractionIndex( refractionIndex )
    {}

    virtual bool Scatter( const Ray &r,
                          const HitRecord &rec,
                          Color &attenuation,
                          Ray &scattered ) const override;

private:

    double  _refractionIndex = 1.0;

    static double reflectance( double cosine, double refractionIndex );
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class DiffuseLight : public Material
{
public:

    DiffuseLight( std::shared_ptr<Texture> tex )
        : _tex( tex )
    {}

    DiffuseLight( const Color &emit )
        : _tex( std::make_shared<SolidColor>( emit ) )
    {}

    Color Emitted( double u, double v, const Point3 &p ) const override;

    virtual bool Scatter( const Ray &r,
                          const HitRecord &rec,
                          Color &attenuation,
                          Ray &scattered ) const override;
private:

    std::shared_ptr<Texture> _tex;
};

