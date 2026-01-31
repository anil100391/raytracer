#include <sphere.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
Sphere::Sphere( const Point3 &center,
                double radius,
                const std::shared_ptr<Material> &mat )
    : _center( center, Vec3( 0, 0, 0 ) ),
      _radius( radius ),
      _mat( mat )
{
    auto rvec = Vec3( _radius, _radius, _radius );
    _bbox = AABB( center - rvec, center + rvec );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
Sphere::Sphere( const Point3 &start,
                const Point3 &end,
                double radius,
                const std::shared_ptr<Material> &mat )
    : _center( start, end - start ),
      _radius( std::fmax( 0, radius ) ),
      _mat( mat )
{
    auto rvec = Vec3( _radius, _radius, _radius );
    AABB box1( _center.at( 0 ) - rvec, _center.at( 0 ) + rvec );
    AABB box2( _center.at( 1 ) - rvec, _center.at( 1 ) + rvec );
    _bbox = AABB( box1, box2 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool Sphere::Hit(const Ray &r, const Interval &RayT, HitRecord &rec) const
{
    const auto ro = r.origin();
    const auto &rd = r.direction();
    auto center    = _center.at( r.time() );
    auto oc = center - ro;
    auto a  = rd.Len2();
    auto h  = Dot( rd, oc );
    auto c  = oc.Len2() - _radius * _radius;
    auto discriminant = h * h - a * c;
    if ( discriminant < 0 )
        return false;

    auto sqrtd = std::sqrt( discriminant );

    auto root = (h - sqrtd) / a;
    if ( !RayT.Surrounds( root ) )
    {
        root = (h + sqrtd) / a;
        if ( !RayT.Surrounds( root ) )
            return false;
    }

    rec.t = root;
    rec.p = r.at( rec.t );
    Vec3 outwardNormal = (rec.p - center) / _radius;
    rec.SetFaceNormal( r, outwardNormal );
    rec.mat = _mat;

    return true;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
AABB Sphere::BoundingBox() const
{
    return _bbox;
}
