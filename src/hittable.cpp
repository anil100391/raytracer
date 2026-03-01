#include <hittable.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
Translate::Translate( std::shared_ptr<Hittable> object, const Vec3 &offset )
    : _object( object ), _offset( offset )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool Translate::Hit( const Ray &r, const Interval &rayT, HitRecord &rec ) const
{
    // Move the ray backwards by the offset
    Ray offsetR( r.origin() - _offset, r.direction(), r.time() );

    // Determine whether an intersection exists along the offset ray (and if so, where)
    if ( !_object->Hit( offsetR, rayT, rec ) )
        return false;

    // Move the intersection point forwards by the offset
    rec.p += _offset;

    return true;
}
 
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
Rotate_Y::Rotate_Y( std::shared_ptr<Hittable> object, double angle )
    : _object( object )
{
    auto radians = DegreesToRadians( angle );
    _sin_theta   = std::sin( radians );
    _cos_theta   = std::cos( radians );
    _bbox        = _object->BoundingBox();

    Point3 min( infinity, infinity, infinity );
    Point3 max( -infinity, -infinity, -infinity );

    for ( int i = 0; i < 2; ++i )
    {
        for ( int j = 0; j < 2; ++j )
        {
            for ( int k = 0; k < 2; ++k )
            {
                auto x = i * _bbox.x.max + (1 - i) * _bbox.x.min;
                auto y = j * _bbox.y.max + (1 - j) * _bbox.y.min;
                auto z = k * _bbox.z.max + (1 - k) * _bbox.z.min;

                auto newx =  _cos_theta * x + _sin_theta * z;
                auto newz = -_sin_theta * x + _cos_theta * z;

                Vec3 tester( newx, y, newz );

                for ( int c = 0; c < 3; ++c )
                {
                    min[c] = std::fmin( min[c], tester[c] );
                    max[c] = std::fmax( max[c], tester[c] );
                }
            }
        }
    }

    _bbox = AABB( min, max );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool Rotate_Y::Hit( const Ray &r, const Interval &rayT, HitRecord &rec ) const
{
    // Transform the Ray from world space to _object space.
    auto origin = Point3( (_cos_theta * r.origin().x()) - (_sin_theta * r.origin().z()),
                          r.origin().y(),
                          (_sin_theta * r.origin().x()) + (_cos_theta * r.origin().z()) );

    auto direction = Vec3( (_cos_theta * r.direction().x()) - (_sin_theta * r.direction().z()),
                           r.direction().y(),
                           (_sin_theta * r.direction().x()) + (_cos_theta * r.direction().z()) );

    Ray rotated_r( origin, direction, r.time() );

    // Determine whether an intersection exists in _object space (and if so, where).
    if ( !_object->Hit( rotated_r, rayT, rec ) )
        return false;

    // Transform the intersection from _object space back to world space.
    rec.p = Point3( (_cos_theta * rec.p.x()) + (_sin_theta * rec.p.z()),
                    rec.p.y(),
                    (-_sin_theta * rec.p.x()) + (_cos_theta * rec.p.z()) );

    rec.normal = Vec3( (_cos_theta * rec.normal.x()) + (_sin_theta * rec.normal.z()),
                       rec.normal.y(),
                       (-_sin_theta * rec.normal.x()) + (_cos_theta * rec.normal.z()) );

    return true;
}
