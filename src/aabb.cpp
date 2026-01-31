#include <aabb.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
const AABB AABB::Empty = AABB( Interval::Empty, Interval::Empty, Interval::Empty );
const AABB AABB::Universe = AABB( Interval::Universe, Interval::Universe, Interval::Universe );

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool AABB::Hit( const Ray &r, Interval rayT ) const
{
    const Point3 &rayOrigin = r.origin();
    const Vec3   &rayDir    = r.direction();

    for ( int axis = 0; axis < 3; ++axis )
    {
        const Interval &ax = AxisInterval( axis );
        const double adinv = 1.0 / rayDir[axis];

        auto t0 = (ax.min - rayOrigin[axis]) * adinv;
        auto t1 = (ax.max - rayOrigin[axis]) * adinv;

        if ( t0 < t1 )
        {
            if ( t0 > rayT.min )
                rayT.min = t0;
            if ( t1 < rayT.max )
                rayT.max = t1;
        }
        else
        {
            if ( t1 > rayT.min )
                rayT.min = t1;
            if ( t0 < rayT.max )
                rayT.max = t0;
        }

        if ( rayT.max <= rayT.min )
            return false;
    }

    return true;
}
