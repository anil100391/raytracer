#pragma once

#include <hittable.h>
#include <material.h>
#include <texture.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class ConstantMedium : public Hittable
{
public:

    ConstantMedium( std::shared_ptr<Hittable> boundary,
                    double density,
                    std::shared_ptr<Texture> tex );

    ConstantMedium( std::shared_ptr<Hittable> boundary,
                    double density,
                    const Color &albedo );

    virtual bool Hit( const Ray &r, const Interval &RayT, HitRecord &rec ) const override;

    virtual AABB BoundingBox() const override { return _boundary->BoundingBox();  }

private:

    std::shared_ptr<Hittable> _boundary;
    double                    _negInvDensity = 0;
    std::shared_ptr<Material> _phaseFunction;
};
