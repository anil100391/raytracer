#include <quad.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
Quad::Quad( const Point3 &Q, const Vec3 &u, const Vec3 &v, std::shared_ptr<Material> mat )
    : _Q( Q ), _u( u ), _v( v ), _mat( mat )
{
    auto n  = Cross( u, v );
    _normal = Normalize( n );
    _D      = Dot( _normal, Q );
    _w      = n / Dot( n, n );

    SetBoundingBox();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool Quad::Hit( const Ray &r, const Interval &rayT, HitRecord &rec ) const
{
    auto denom = Dot( _normal, r.direction() );

    if ( std::fabs( denom ) < 1e-8 )
    {
        return false;
    }

    auto t = (_D - Dot( _normal, r.origin() )) / denom;
    if ( !rayT.Contains( t ) )
    {
        return false;
    }

    auto intersection = r.at( t );
    Vec3 planarHitPTVector = intersection - _Q;
    auto alpha = Dot( _w, Cross( planarHitPTVector, _v ) );
    auto beta = Dot( _w, Cross( _u, planarHitPTVector ) );

    if ( !IsInterior( alpha, beta, rec ) )
    {
        return false;
    }

    rec.t = t;
    rec.p = intersection;
    rec.mat = _mat;
    rec.SetFaceNormal( r, _normal );

    return true;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool Quad::IsInterior( double a, double b, HitRecord &rec ) const
{
    Interval unitInterval( 0, 1 );

    if ( !unitInterval.Contains( a ) || !unitInterval.Contains( b ) )
    {
        return false;
    }

    rec.u = a;
    rec.v = b;
    return true;
}
