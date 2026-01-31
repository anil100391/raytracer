#pragma once

#include <memory>

#include <aabb.h>
#include <hittable.h>
#include <hittableList.h>

class BVHNode : public Hittable
{
public:

    BVHNode( HittableList list );

    BVHNode( std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end );

    bool Hit( const Ray &r, const Interval &RayT, HitRecord &rec ) const override;

    AABB BoundingBox() const  override { return bbox; }

private:

    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB bbox;
};

