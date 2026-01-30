#include <material.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool Lambertian::Scatter( const Ray &r,
                          const HitRecord &rec,
                          Color &attenuation,
                          Ray &scattered ) const
{
    auto scatterDirection = rec.normal + RandomUnitVector();
    if ( scatterDirection.NearZero() )
        scatterDirection = rec.normal;

    scattered = Ray( rec.p, scatterDirection );
    attenuation = _albedo;
    return true;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool Metal::Scatter( const Ray &r,
                     const HitRecord &rec,
                     Color &attenuation,
                     Ray &scattered ) const
{
    Vec3 reflected = Reflect( r.direction(), rec.normal);
    reflected      = Normalize( reflected ) + (_fuzz * RandomUnitVector());
    scattered      = Ray( rec.p, reflected );
    attenuation    = _albedo;
    return (Dot( scattered.direction(), rec.normal ) > 0);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool Dielectric::Scatter( const Ray &r,
                          const HitRecord &rec,
                          Color &attenuation,
                          Ray &scattered ) const
{
    attenuation = Color( 1, 1, 1 );
    double ri = rec.frontFace ? (1.0 / _refractionIndex) : _refractionIndex;

    Vec3 unitDirection = Normalize( r.direction() );
    double cosTheta = std::fmin( Dot( -unitDirection, rec.normal ), 1.0 );
    double sinTheta = std::sqrt( 1.0 - cosTheta * cosTheta );

    bool cantRefract = (ri * sinTheta > 1.0);
    Vec3 direction;

    if ( cantRefract || reflectance( cosTheta, ri ) > RandomDouble() )
        direction = Reflect( unitDirection, rec.normal );
    else
        direction = Refract( unitDirection, rec.normal, ri );

    scattered = Ray( rec.p, direction );
    return true;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
double Dielectric::reflectance( double cosine, double refractionIndex )
{
    auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow( (1 - cosine), 5 );
}
