#include <constantMedium.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
ConstantMedium::ConstantMedium( std::shared_ptr<Hittable> boundary,
                                double density,
                                std::shared_ptr<Texture> tex )
    : _boundary( boundary ),
      _negInvDensity( -1 / density ),
      _phaseFunction( std::make_shared<Isotropic>( tex ) )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
ConstantMedium::ConstantMedium( std::shared_ptr<Hittable> boundary,
                                double density,
                                const Color &albedo )
    : _boundary( boundary ),
      _negInvDensity( -1 / density ),
      _phaseFunction( std::make_shared<Isotropic>( albedo ) )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool ConstantMedium::Hit( const Ray &r,
                          const Interval &rayT,
                          HitRecord &rec ) const
{
    HitRecord rec1, rec2;

    if ( !_boundary->Hit( r, Interval::Universe, rec1 ) )
        return false;

    if ( !_boundary->Hit( r, Interval( rec1.t + 0.0001, infinity ), rec2 ) )
        return false;

    if ( rec1.t < rayT.min )
        rec1.t = rayT.min;
    if ( rec2.t > rayT.max )
        rec2.t = rayT.max;

    if ( rec1.t >= rec2.t )
        return false;

    auto rayLength = r.direction().Length();
    auto distanceInsideBoundary = (rec2.t - rec1.t) * rayLength;
    auto hitDistance = _negInvDensity * std::log( RandomDouble() );

    if ( hitDistance > distanceInsideBoundary )
        return false;

    rec.t = rec1.t + hitDistance / rayLength;
    rec.p = r.at( rec.t );

    rec.normal = Vec3( 1, 0, 0 );
    rec.frontFace = true;
    rec.mat = _phaseFunction;

    return true;
}
