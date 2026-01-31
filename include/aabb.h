#pragma once

#include <vec3.h>
#include <ray.h>
#include <interval.h>

class AABB
{
public:

    Interval x, y, z;

    AABB() = default;

    AABB( const Interval &xi, const Interval &yi, const Interval &zi )
        : x( xi ), y( yi ), z( zi )
    {}

    AABB( const Point3 &a, const Point3 &b )
    {
        x = (a[0] <= b[0]) ? Interval( a[0], b[0] ) : Interval( b[0], a[0] );
        y = (a[1] <= b[1]) ? Interval( a[1], b[1] ) : Interval( b[1], a[1] );
        z = (a[2] <= b[2]) ? Interval( a[2], b[2] ) : Interval( b[2], a[2] );
    }

    AABB( const AABB &box0, const AABB &box1 )
    {
        x = Interval( box0.x, box1.x );
        y = Interval( box0.y, box1.y );
        z = Interval( box0.z, box1.z );
    }

    [[nodiscard]] const Interval &AxisInterval( int n ) const noexcept
    {
        if ( n == 1 )
            return y;
        if ( n == 2 )
            return z;

        return x;
    }

    bool Hit( const Ray &r, Interval rayT ) const;

private:
};
