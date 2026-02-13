#pragma once

#include <vector>
#include <color.h>
#include <hittable.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Camera
{
public:

    double       aspectRatio     = 1.0;
    unsigned int imageWidth      = 100u;
    unsigned int samplesPerPixel = 10u;
    unsigned int maxDepth        = 10u;
    Color        backGround;

    double       vfov            = 90; // Vertical view angle (field of view)
    Point3       lookFrom        = Point3( 0, 0, 0 );
    Point3       lookAt          = Point3( 0, 0, -1 );
    Vec3         vup             = Point3( 0, 1, 0 );

    double       defocusAngle    = 0.0;
    double       focusDist       = 10.0;

    [[nodiscard]] unsigned int ImageWidth() const noexcept { return imageWidth; }
    [[nodiscard]] unsigned int ImageHeight() const noexcept { return imageHeight; }

    void Render( const Hittable &world, std::vector<uint8_t> &image );

private:

    void Initialize();

    [[nodiscard]] Point3 DefocusDiskSample() const noexcept;
    [[nodiscard]] Vec3 SampleSquare() const noexcept;
    [[nodiscard]] Ray GetRay( unsigned int col, unsigned int row ) const noexcept;
    [[nodiscard]] Color RayColor( const Ray &r, int depth, const Hittable &world ) const noexcept;

    unsigned int imageHeight       = 100u;
    double       pixelSamplesScale = 1.0;
    Point3       center;
    Point3       pixel00Loc;
    Vec3         pixelDeltaU;
    Vec3         pixelDeltaV;
    Vec3         u, v, w;                   // camera basis vectors
    Vec3         defocusDiskU;
    Vec3         defocusDiskV;
};
