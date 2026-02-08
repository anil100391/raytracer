#include <vector>
#include <iostream>

#include <ppmio.h>
#include <vec3.h>
#include <ray.h>
#include <bvh.h>
#include <quad.h>
#include <color.h>
#include <camera.h>
#include <sphere.h>
#include <rtDefs.h>
#include <texture.h>
#include <material.h>
#include <hittableList.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static void BookCoverImage( bool renderBouncingSpheres )
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
                    if ( renderBouncingSpheres )
                    {
                        auto center2 = center + Vec3( 0.0, RandomDouble( 0.0, 0.5 ), 0.0 );
                        world.Add( std::make_shared<Sphere>( center, center2, 0.2, sphereMaterial ) );
                    }
                    else
                    {
                        world.Add( std::make_shared<Sphere>( center, 0.2, sphereMaterial ) );
                    }
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

    world = HittableList( std::make_shared<BVHNode>( world ) );

    Camera camera;

    camera.aspectRatio     = 16.0 / 9.0;
    camera.imageWidth      = 1920 * 2;
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
static void MaterialTest()
{
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
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static void Earth()
{
    auto earthTexture = std::make_shared<ImageTexture>( "images/earthmap.jpg" );
    auto earthSurface = std::make_shared<Lambertian>( earthTexture );
    auto globe        = std::make_shared<Sphere>( Point3(), 2, earthSurface );

    Camera camera;
    camera.aspectRatio     = 16.0 / 9.0;
    camera.imageWidth      = 400u;
    camera.samplesPerPixel = 100u;
    camera.maxDepth        = 50u;

    camera.vfov            = 20.0;
    camera.lookFrom        = Point3( 0, 0, 12 );
    camera.lookAt          = Point3( 0, 0, 0 );
    camera.vup             = Vec3( 0, 1, 0 );

    camera.defocusAngle    = 0.0;

    std::vector<uint8_t> image;
    camera.Render( HittableList( globe ), image );
    rtPPMio::WritePPM( "earth.ppm", camera.ImageWidth(), camera.ImageHeight(), image.data() );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static void PerlinSpheres()
{
    HittableList world;

    auto perText = std::make_shared<NoiseTexture>( 4 );
    world.Add( std::make_shared<Sphere>( Point3( 0, -1000, 0 ), 1000, std::make_shared<Lambertian>( perText ) ) );
    world.Add( std::make_shared<Sphere>( Point3( 0, 2, 0 ), 2, std::make_shared<Lambertian>( perText ) ) );

    Camera camera;
    camera.aspectRatio     = 16.0 / 9.0;
    camera.imageWidth      = 400u;
    camera.samplesPerPixel = 100u;
    camera.maxDepth        = 50u;

    camera.vfov            = 20.0;
    camera.lookFrom        = Point3( 13, 2, 3 );
    camera.lookAt          = Point3( 0, 0, 0 );
    camera.vup             = Vec3( 0, 1, 0 );

    camera.defocusAngle    = 0.0;

    std::vector<uint8_t> image;
    camera.Render( HittableList( world ), image );
    rtPPMio::WritePPM( "perlin.ppm", camera.ImageWidth(), camera.ImageHeight(), image.data() );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static void Quads()
{
    HittableList world;

    // Materials
    auto leftRed     = std::make_shared<Lambertian>( Color( 1.0, 0.2, 0.2 ) );
    auto backGreen   = std::make_shared<Lambertian>( Color( 0.2, 1.0, 0.2 ) );
    auto rightBlue   = std::make_shared<Lambertian>( Color( 0.2, 0.2, 1.0 ) );
    auto upperOrange = std::make_shared<Lambertian>( Color( 1.0, 0.5, 0.0 ) );
    auto lowerTeal   = std::make_shared<Lambertian>( Color( 0.2, 0.8, 0.8 ) );

    // Quads
    world.Add( std::make_shared<Quad>( Point3( -3, -2, 5 ), Vec3( 0, 0, -4 ), Vec3( 0, 4, 0 ), leftRed ) );
    world.Add( std::make_shared<Quad>( Point3( -2, -2, 0 ), Vec3( 4, 0, 0 ), Vec3( 0, 4, 0 ), backGreen ) );
    world.Add( std::make_shared<Quad>( Point3( 3, -2, 1 ), Vec3( 0, 0, 4 ), Vec3( 0, 4, 0 ), rightBlue ) );
    world.Add( std::make_shared<Quad>( Point3( -2, 3, 1 ), Vec3( 4, 0, 0 ), Vec3( 0, 0, 4 ), upperOrange ) );
    world.Add( std::make_shared<Quad>( Point3( -2, -3, 5 ), Vec3( 4, 0, 0 ), Vec3( 0, 0, -4 ), lowerTeal ) );

    Camera camera;

    camera.aspectRatio = 1.0;
    camera.imageWidth = 400;
    camera.samplesPerPixel = 100;
    camera.maxDepth = 50;

    camera.vfov = 80;
    camera.lookFrom = Point3( 0, 0, 9 );
    camera.lookAt = Point3( 0, 0, 0 );
    camera.vup = Vec3( 0, 1, 0 );

    camera.defocusAngle = 0;

    std::vector<uint8_t> image;
    camera.Render( HittableList( world ), image );
    rtPPMio::WritePPM( "quads.ppm", camera.ImageWidth(), camera.ImageHeight(), image.data() );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
int main( int argc, const char *argv[] )
{
    switch ( 11 )
    {
        case 0:
            MaterialTest();
            break;
        case 1:
            BookCoverImage( true );
            break;
        case 3:
            Earth();
            break;
        case 10:
            PerlinSpheres();
            break;
        case 11:
            Quads();
            break;
        default:
            break;
    }

    return 0u;
}
