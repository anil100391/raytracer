#pragma once

#include <vec3.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Ray
{
public:

    Ray() = default;
    Ray( const Point3 &point, const Vec3 &direction ) : orig( point ), dir( direction ) {}

    [[nodiscard]] const Point3& origin() const noexcept { return orig; }
    [[nodiscard]] const Vec3& direction() const noexcept { return dir; }

    [[nodiscard]] Point3 at( double t ) const noexcept
    {
        return orig + t * dir;
    }

private:

    Point3 orig;
    Vec3   dir;
};
