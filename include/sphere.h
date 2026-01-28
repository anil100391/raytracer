#pragma once

#include <memory>

#include <hittable.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Sphere : public Hittable
{
public:

    Sphere( const Point3 &center, double radius, const std::shared_ptr<Material> &mat )
        : _center( center ), _radius( radius ), _mat( mat )
    {}

    virtual bool Hit( const Ray &r, const Interval &RayT, HitRecord &rec ) const override;

private:

    Point3                    _center;
    double                    _radius = 1.0;
    std::shared_ptr<Material> _mat;
};
