#pragma once

#include <memory>

#include <aabb.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Material;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class HitRecord
{
public:

    Point3 p;
    Vec3   normal;
    std::shared_ptr<Material> mat;
    double t = 0.0;
    double u = 0.0;
    double v = 0.0;
    bool frontFace = false;

    void SetFaceNormal( const Ray &r, const Vec3 &outwardNormal )
    {
        frontFace = Dot( r.direction(), outwardNormal ) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Hittable
{
public:

    virtual ~Hittable() = default;

    virtual bool Hit( const Ray &r, const Interval &RayT, HitRecord &rec ) const = 0;

    virtual AABB BoundingBox() const = 0;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Translate : public Hittable
{
public:

    Translate( std::shared_ptr<Hittable> object, const Vec3 &offset );

    virtual bool Hit( const Ray &r, const Interval &RayT, HitRecord &rec ) const override;

    virtual AABB BoundingBox() const override { return _bbox; }
private:

    std::shared_ptr<Hittable> _object;
    Vec3                      _offset;
    AABB                      _bbox;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Rotate_Y :public Hittable
{
public:

    Rotate_Y( std::shared_ptr<Hittable> object, double angle );

    virtual bool Hit( const Ray &r, const Interval &RayT, HitRecord &rec ) const override;

    virtual AABB BoundingBox() const override { return _bbox; }

private:

    std::shared_ptr<Hittable> _object;
    double                    _sin_theta = 0;
    double                    _cos_theta = 1;
    AABB                      _bbox;
};
