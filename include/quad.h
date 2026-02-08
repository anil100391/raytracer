#pragma once

#include <hittable.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Quad : public Hittable
{
public:

    Quad( const Point3 &Q, const Vec3 &u, const Vec3 &v, std::shared_ptr<Material> mat );

    virtual void SetBoundingBox()
    {
        auto boxDiagonal1 = AABB( _Q, _Q + _u + _v );
        auto boxDiagonal2 = AABB( _Q + _u, _Q + _v );
        _bbox = AABB( boxDiagonal1, boxDiagonal2 );
    }

    [[nodiscard]] AABB BoundingBox() const noexcept { return _bbox; }

    virtual bool Hit( const Ray &r, const Interval &rayT, HitRecord &rec ) const  override;
    virtual bool IsInterior( double a, double b, HitRecord &rec ) const;

private:

    Point3                    _Q;
    Vec3                      _u, _v;
    Vec3                      _w;
    std::shared_ptr<Material> _mat;
    AABB                      _bbox;
    Vec3                      _normal;
    double                    _D;
};
