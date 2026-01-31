#include <algorithm>

#include <bvh.h>
#include <interval.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static bool BoxCompare( const std::shared_ptr<Hittable> a,
                        const std::shared_ptr<Hittable> b,
                        int axisIndex )
{
    auto aAxisInterval = a->BoundingBox().AxisInterval( axisIndex );
    auto bAxisInterval = b->BoundingBox().AxisInterval( axisIndex );
    return aAxisInterval.min < bAxisInterval.min;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static bool BoxXCompare( const std::shared_ptr<Hittable> a,
                         const std::shared_ptr<Hittable> b )
{
    return BoxCompare( a, b, 0 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static bool BoxYCompare( const std::shared_ptr<Hittable> a,
                         const std::shared_ptr<Hittable> b )
{
    return BoxCompare( a, b, 1 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static bool BoxZCompare( const std::shared_ptr<Hittable> a,
                         const std::shared_ptr<Hittable> b )
{
    return BoxCompare( a, b, 2 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
BVHNode::BVHNode( HittableList list )
    : BVHNode( list.objects, 0, list.objects.size() )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
BVHNode::BVHNode( std::vector<std::shared_ptr<Hittable>> &objects,
                  size_t start,
                  size_t end )
{
    bbox = AABB::Empty;
    for ( auto ii = start; ii < end; ++ii )
    {
        bbox = AABB( bbox, objects[ii]->BoundingBox() );
    }

    int axis = bbox.LongestAxis();

    auto comparator = (axis == 0) ? BoxXCompare
                                  : (axis == 1) ? BoxYCompare
                                                : BoxZCompare;

    size_t objectSpan = end - start;
    if ( objectSpan == 1 )
    {
        left = right = objects[start];
    }
    else if ( objectSpan == 2 )
    {
        left = objects[start];
        right = objects[start + 1];
    }
    else
    {
        std::sort( std::begin( objects ) + start,
                   std::begin( objects ) + end,
                   comparator );
        auto mid = start + objectSpan / 2;
        left = std::make_shared<BVHNode>( objects, start, mid );
        right = std::make_shared<BVHNode>( objects, mid, end );
    }

    bbox = AABB( left->BoundingBox(), right->BoundingBox() );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool BVHNode::Hit( const Ray &r, const Interval &rayT, HitRecord &rec ) const
{
    auto localRayT = rayT;
    if ( !bbox.Hit( r, localRayT ) )
        return false;

    bool hitLeft = left->Hit( r, localRayT, rec );
    bool hitRight = right->Hit( r, Interval( localRayT.min, hitLeft ? rec.t : localRayT.max ), rec );

    return hitLeft || hitRight;
}
