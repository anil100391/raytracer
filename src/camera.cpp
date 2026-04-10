#include <ctime>
#include <format>
#include <camera.h>
#include <material.h>
#include <rtSTBImage.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void Camera::Render( const Hittable &world, rtImage &image )
{
    auto begin = clock();

    Initialize();

    image.Resize(imageWidth, imageHeight);
    unsigned char* data = image.PixelData();
    for ( auto ii = 0u; ii < imageHeight; ++ii )
    {
        std::clog << "\rScalines remaining: " << imageHeight - ii << " " << std::flush;
        for ( auto jj = 0u; jj < imageWidth; ++jj )
        {
            Color pixelColor( 0, 0, 0 );
            for ( auto sample = 0u; sample < samplesPerPixel; ++sample )
            {
                Ray r = GetRay( jj, ii );
                pixelColor += RayColor( r, maxDepth, world );
            }

            pixelColor *= pixelSamplesScale;

            auto r = LinearToGamma( pixelColor.x() );
            auto g = LinearToGamma( pixelColor.y() );
            auto b = LinearToGamma( pixelColor.z() );

            static const Interval intensity( 0.0, 0.999 );
            data[0] = uint8_t( 256 * intensity.Clamp( r ) );
            data[1] = uint8_t( 256 * intensity.Clamp( g ) );
            data[2] = uint8_t( 256 * intensity.Clamp( b ) );

            data += 3u;
        }
    }

    std::cout << std::format( "Rendered in {}s\n", (1.0 * (clock() - begin)) / CLOCKS_PER_SEC );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void Camera::Initialize()
{
    // Coordinate System is as described below
    // Camera is at origin, camera up is along Y, camera is facing along -Z and
    // camera right is along X
    // Viewport (image pixel grid is 1 unit away from camera
    imageHeight = static_cast<unsigned int>(imageWidth / aspectRatio);
    imageHeight = imageHeight < 1 ? 1u : imageHeight;

    pixelSamplesScale = 1.0 / samplesPerPixel;

    center              = lookFrom;

    auto theta          = DegreesToRadians(vfov);
    auto h              = std::tan( theta / 2 );
    auto viewportHeight = 2 * h * focusDist;
    auto viewportWidth  = viewportHeight * ((double)imageWidth / imageHeight);

    // basis vectors
    w = Normalize( lookFrom - lookAt );
    u = Normalize( Cross( vup, w ) );
    v = Cross( w, u );

    auto viewportU = viewportWidth * u;
    auto viewportV = viewportHeight * -v;

    pixelDeltaU = viewportU / imageWidth;
    pixelDeltaV = viewportV / imageHeight;

    auto viewportUpperLeft = center - (focusDist * w) - viewportU / 2 - viewportV / 2;
    pixel00Loc             = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

    auto defocusRadius     = focusDist * std::tan( DegreesToRadians( defocusAngle / 2 ) );
    defocusDiskU           = u * defocusRadius;
    defocusDiskU           = v * defocusRadius;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
Point3 Camera::DefocusDiskSample() const noexcept
{
    auto p = RandomInUnitDisk();
    return center + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
Vec3 Camera::SampleSquare() const noexcept
{
    return Vec3( RandomDouble() - 0.5, RandomDouble() - 0.5, 0.0 );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
Ray Camera::GetRay( unsigned int col, unsigned int row ) const noexcept
{
    auto offset = SampleSquare();
    auto pixelSample = pixel00Loc + ((col + offset.x()) * pixelDeltaU)
                                  + ((row + offset.y()) * pixelDeltaV);
    auto rayOrigin    = (defocusAngle <= 0) ? center : DefocusDiskSample();
    auto rayDirection = pixelSample - rayOrigin;
    auto rayTime      = RandomDouble();
    return Ray( rayOrigin, rayDirection, rayTime );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
Color Camera::RayColor( const Ray &r,
                        int depth,
                        const Hittable &world ) const noexcept
{
    if ( depth <= 0 )
        return Color( 0, 0, 0 );

    HitRecord rec;
    if ( !world.Hit( r, Interval( 0.001, infinity ), rec ) )
    {
        return backGround;
    }

    Ray scattered;
    Color attenuation;
    Color colorFromEmission = rec.mat->Emitted( rec.u, rec.v, rec.p );

    if ( !rec.mat->Scatter( r, rec, attenuation, scattered ) )
        return colorFromEmission;

    Color colorFromScatter = attenuation * RayColor( scattered, depth - 1, world );
    return colorFromEmission + colorFromScatter;
}
