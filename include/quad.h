#pragma once

#include <hittable.h>
#include <hittableList.h>

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

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline std::shared_ptr<HittableList> Box( const Point3 &a, const Point3 &b, std::shared_ptr<Material> mat )
{
    auto sides = std::make_shared<HittableList>();

    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = Point3( std::fmin( a.x(), b.x() ), std::fmin( a.y(), b.y() ), std::fmin( a.z(), b.z() ) );
    auto max = Point3( std::fmax( a.x(), b.x() ), std::fmax( a.y(), b.y() ), std::fmax( a.z(), b.z() ) );

    auto dx = Vec3( max.x() - min.x(), 0.0, 0.0 );
    auto dy = Vec3( 0.0, max.y() - min.y(), 0.0 );
    auto dz = Vec3( 0.0, 0.0, max.z() - min.z() );

    sides->Add( std::make_shared<Quad>( Point3( min.x(), min.y(), max.z() ), dx, dy, mat ) ); // front
    sides->Add( std::make_shared<Quad>( Point3( max.x(), min.y(), max.z() ), -dz, dy, mat ) ); // right
    sides->Add( std::make_shared<Quad>( Point3( max.x(), min.y(), min.z() ), -dx, dy, mat ) ); // back
    sides->Add( std::make_shared<Quad>( Point3( min.x(), min.y(), min.z() ), dz, dy, mat ) ); // left
    sides->Add( std::make_shared<Quad>( Point3( min.x(), max.y(), max.z() ), dx, -dz, mat ) ); // top
    sides->Add( std::make_shared<Quad>( Point3( min.x(), min.y(), min.z() ), dx, dz, mat ) );

    return sides;
}
