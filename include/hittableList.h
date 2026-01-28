#pragma once

#include <vector>
#include <memory>
#include <hittable.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class HittableList : public Hittable
{
public:

    std::vector<std::shared_ptr<Hittable>> objects;

    HittableList() = default;
    HittableList( const std::shared_ptr<Hittable> &object )
    {
        Add( object );
    }

    void Clear()
    {
        objects.clear();
    }

    void Add( const std::shared_ptr<Hittable> &object )
    {
        objects.push_back( object );
    }

    bool Hit( const Ray &r, const Interval &RayT, HitRecord &rec ) const override
    {
        HitRecord tempRec;
        bool hitAnything = false;
        auto closestSoFar = RayT.max;

        for ( const auto &object : objects )
        {
            if ( object->Hit( r, Interval( RayT.min, closestSoFar ), tempRec ) )
            {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }
};
