#include <sphere.h>

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
