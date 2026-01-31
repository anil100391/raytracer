#include <vector>
#include <iostream>

#include <ppmio.h>
#include <vec3.h>
#include <ray.h>
#include <color.h>
#include <camera.h>
#include <sphere.h>
#include <rtDefs.h>
#include <material.h>
#include <hittableList.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#define RENDER_BOOK_COVER 1

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static void BookCoverImage()
{
    HittableList world;

    auto groundMaterial = std::make_shared<Lambertian>( Color( 0.5, 0.5, 0.5 ) );
    world.Add( std::make_shared<Sphere>( Point3( 0, -1000, 0 ), 1000, groundMaterial ) );

    for ( int a = -11; a < 11; a++ )
    {
        for ( int b = -11; b < 11; b++ )
        {
            auto chooseMat = RandomDouble();
            Point3 center( a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble() );

            if ( (center - Point3( 4.0, 0.2, 0.0 )).Length() > 0.9 )
            {
                std::shared_ptr<Material> sphereMaterial;

                if ( chooseMat < 0.8 )
                {
                    // diffuse
                    auto albedo = Color::Random() * Color::Random();
                    sphereMaterial = std::make_shared<Lambertian>( albedo );
                    auto center2   = center + Vec3( 0.0, RandomDouble( 0.0, 0.5 ), 0.0 );
                    world.Add( std::make_shared<Sphere>( center, center2, 0.2, sphereMaterial ) );
                }
                else if ( chooseMat < 0.95 )
                {
                    // Metal
                    auto albedo = Color::Random( 0.5, 1 );
                    auto fuzz = RandomDouble( 0, 0.5 );
                    sphereMaterial = std::make_shared<Metal>( albedo, fuzz );
                    world.Add( std::make_shared<Sphere>( center, 0.2, sphereMaterial ) );
                }
                else
                {
                    // glass
                    sphereMaterial = std::make_shared<Dielectric>( 1.5 );
                    world.Add( std::make_shared<Sphere>( center, 0.2, sphereMaterial ) );
                }
            }
        }
    }

    auto Material1 = std::make_shared<Dielectric>( 1.5 );
    world.Add( std::make_shared<Sphere>( Point3( 0, 1, 0 ), 1.0, Material1 ) );

    auto Material2 = std::make_shared<Lambertian>( Color( 0.4, 0.2, 0.1 ) );
    world.Add( std::make_shared<Sphere>( Point3( -4, 1, 0 ), 1.0, Material2 ) );

    auto Material3 = std::make_shared<Metal>( Color( 0.7, 0.6, 0.5 ), 0.0 );
    world.Add( std::make_shared<Sphere>( Point3( 4, 1, 0 ), 1.0, Material3 ) );

    Camera camera;

    camera.aspectRatio     = 16.0 / 9.0;
    camera.imageWidth      = 400;
    camera.samplesPerPixel = 100u;
    camera.maxDepth        = 50u;

    camera.vfov            = 20;
    camera.lookFrom        = Point3( 13, 2, 3 );
    camera.lookAt          = Point3( 0, 0, 0 );
    camera.vup             = Vec3( 0, 1, 0 );

    camera.defocusAngle    = 0.6;
    camera.focusDist       = 10.0;

    std::vector<uint8_t> image;
    camera.Render(world, image);
    rtPPMio::WritePPM( "BookCover.ppm", camera.ImageWidth(), camera.ImageHeight(), image.data() );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
int main( int argc, const char *argv[] )
{
#if RENDER_BOOK_COVER
    BookCoverImage();
#else
    auto MaterialGround = std::make_shared<Lambertian>( Color( 0.8, 0.8, 0.0 ) );
    auto MaterialCenter = std::make_shared<Lambertian>( Color( 0.1, 0.2, 0.5 ) );
    auto MaterialLeft   = std::make_shared<Dielectric>( 1.50 );
    auto MaterialBubble = std::make_shared<Dielectric>( 1.0 / 1.50 );
    auto MaterialRight  = std::make_shared<Metal>( Color( 0.8, 0.6, 0.2 ), 1.0 );

    HittableList world;
    world.Add( std::make_shared<Sphere>( Point3( 0.0, -100.5, -1.0 ), 100, MaterialGround ) );
    world.Add( std::make_shared<Sphere>( Point3( 0.0, 0.0, -1.2 ), 0.5, MaterialCenter ) );
    world.Add( std::make_shared<Sphere>( Point3( -1.0, 0.0, -1.0 ), 0.5, MaterialLeft ) );
    world.Add( std::make_shared<Sphere>( Point3( -1.0, 0.0, -1.0 ), 0.4, MaterialBubble ) );
    world.Add( std::make_shared<Sphere>( Point3( 1.0, 0.0, -1.0 ), 0.5, MaterialRight ) );

    Camera camera;
    camera.aspectRatio     = 16.0 / 9.0;
    camera.imageWidth      = 400u;
    camera.samplesPerPixel = 100u;
    camera.maxDepth        = 50u;

    camera.vfov            = 20.0;
    camera.lookFrom        = Point3(-2, 2, 1);
    camera.lookAt          = Point3( 0, 0, -1 );
    camera.vup             = Vec3( 0, 1, 0 );

    camera.defocusAngle    = 10.0;
    camera.focusDist       = 3.4;

    std::vector<uint8_t> image;
    camera.Render(world, image);
    rtPPMio::WritePPM( "test.ppm", camera.ImageWidth(), camera.ImageHeight(), image.data() );
#endif // RENDER_BOOK_COVER

    return 0u;
}
