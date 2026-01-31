#pragma once

#include <vec3.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Ray
{
public:

    Ray() = default;

    Ray( const Point3 &point,
         const Vec3 &direction,
         double time )
        : orig( point ), dir( direction ), tm( time )
    {}

    Ray( const Point3 &point,
         const Vec3 &direction )
        : orig( point ), dir( direction )
    {}

    [[nodiscard]] const Point3& origin() const noexcept { return orig; }
    [[nodiscard]] const Vec3& direction() const noexcept { return dir; }

    [[nodiscard]] Point3 at( double t ) const noexcept
    {
        return orig + t * dir;
    }

    [[nodiscard]] double time() const noexcept { return tm; }

private:

    Point3 orig;       // ray origin
    Vec3   dir;        // ray direction
    double tm   = 0.0; // time
};
