#pragma once

#include <memory>

#include <hittable.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Sphere : public Hittable
{
public:

    // Stationary sphere
    Sphere( const Point3 &center,
            double radius,
            const std::shared_ptr<Material> &mat )
        : _center( center, Vec3( 0, 0, 0 ) ),
          _radius( radius ),
          _mat( mat )
    {}

    // Moving sphere
    Sphere( const Point3 &start,
            const Point3 &end,
            double radius,
            const std::shared_ptr<Material> &mat )
        : _center( start, end - start ),
          _radius( radius ),
          _mat( mat )
    {}

    virtual bool Hit( const Ray &r, const Interval &RayT, HitRecord &rec ) const override;

private:

    // sphere starts at _center.origin() at t = 0 and moves to _center.origin()
    // + _center.dir() at t = 1
    Ray                       _center;
    double                    _radius = 1.0;
    std::shared_ptr<Material> _mat;
};
