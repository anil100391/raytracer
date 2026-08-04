#include <memory>
#include <thread>
#include <chrono>

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
#include <constantMedium.h>

#include <tinygraphics/app.h>
#include <tinygraphics/shader.h>
#include <tinygraphics/camera.h>
#include <tinygraphics/texture.h>
#include <tinygraphics/utils/meshgl.h>
#include <tinygraphics/utils/meshbufferobjects.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glad/glad.h>

#include <tinygraphics/renderer.h>

#include <tinygraphics/vertexbufferlayout.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static void
BookCoverImage( Camera &camera, rtImage &image, bool renderBouncingSpheres )
{
    HittableList world;

    auto groundMaterial =
        std::make_shared<Lambertian>( Color( 0.5, 0.5, 0.5 ) );
    world.Add( std::make_shared<Sphere>(
        Point3( 0, -1000, 0 ), 1000, groundMaterial ) );

    for ( int a = -11; a < 11; a++ )
    {
        for ( int b = -11; b < 11; b++ )
        {
            auto   chooseMat = RandomDouble();
            Point3 center(
                a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble() );

            if ( ( center - Point3( 4.0, 0.2, 0.0 ) ).Length() > 0.9 )
            {
                std::shared_ptr<Material> sphereMaterial;

                if ( chooseMat < 0.8 )
                {
                    // diffuse
                    auto albedo    = Color::Random() * Color::Random();
                    sphereMaterial = std::make_shared<Lambertian>( albedo );
                    if ( renderBouncingSpheres )
                    {
                        auto center2 =
                            center + Vec3( 0.0, RandomDouble( 0.0, 0.5 ), 0.0 );
                        world.Add( std::make_shared<Sphere>(
                            center, center2, 0.2, sphereMaterial ) );
                    }
                    else
                    {
                        world.Add( std::make_shared<Sphere>(
                            center, 0.2, sphereMaterial ) );
                    }
                }
                else if ( chooseMat < 0.95 )
                {
                    // Metal
                    auto albedo    = Color::Random( 0.5, 1 );
                    auto fuzz      = RandomDouble( 0, 0.5 );
                    sphereMaterial = std::make_shared<Metal>( albedo, fuzz );
                    world.Add( std::make_shared<Sphere>(
                        center, 0.2, sphereMaterial ) );
                }
                else
                {
                    // glass
                    sphereMaterial = std::make_shared<Dielectric>( 1.5 );
                    world.Add( std::make_shared<Sphere>(
                        center, 0.2, sphereMaterial ) );
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

    camera.aspectRatio     = 16.0 / 9.0;
    camera.imageWidth      = 1920;
    camera.samplesPerPixel = 100u;
    camera.maxDepth        = 50u;
    camera.backGround      = Color( 0.70, 0.80, 1.00 );

    camera.vfov     = 20;
    camera.lookFrom = Point3( 13, 2, 3 );
    camera.lookAt   = Point3( 0, 0, 0 );
    camera.vup      = Vec3( 0, 1, 0 );

    camera.defocusAngle = 0.6;
    camera.focusDist    = 10.0;

    camera.Render( world, image );
    image.Save( "renders/BookCover.png" );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static void MaterialTest( Camera &camera, rtImage &image )
{
    auto MaterialGround =
        std::make_shared<Lambertian>( Color( 0.8, 0.8, 0.0 ) );
    auto MaterialCenter =
        std::make_shared<Lambertian>( Color( 0.1, 0.2, 0.5 ) );
    auto MaterialLeft   = std::make_shared<Dielectric>( 1.50 );
    auto MaterialBubble = std::make_shared<Dielectric>( 1.0 / 1.50 );
    auto MaterialRight = std::make_shared<Metal>( Color( 0.8, 0.6, 0.2 ), 1.0 );

    HittableList world;
    world.Add( std::make_shared<Sphere>(
        Point3( 0.0, -100.5, -1.0 ), 100, MaterialGround ) );
    world.Add( std::make_shared<Sphere>(
        Point3( 0.0, 0.0, -1.2 ), 0.5, MaterialCenter ) );
    world.Add( std::make_shared<Sphere>(
        Point3( -1.0, 0.0, -1.0 ), 0.5, MaterialLeft ) );
    world.Add( std::make_shared<Sphere>(
        Point3( -1.0, 0.0, -1.0 ), 0.4, MaterialBubble ) );
    world.Add( std::make_shared<Sphere>(
        Point3( 1.0, 0.0, -1.0 ), 0.5, MaterialRight ) );

    camera.aspectRatio     = 16.0 / 9.0;
    camera.imageWidth      = 400u;
    camera.samplesPerPixel = 100u;
    camera.maxDepth        = 50u;
    camera.backGround      = Color( 0.70, 0.80, 1.00 );

    camera.vfov     = 20.0;
    camera.lookFrom = Point3( -2, 2, 1 );
    camera.lookAt   = Point3( 0, 0, -1 );
    camera.vup      = Vec3( 0, 1, 0 );

    camera.defocusAngle = 10.0;
    camera.focusDist    = 3.4;

    camera.Render( world, image );
    image.Save( "renders/test.png" );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static void Earth( Camera &camera, rtImage &image )
{
    auto earthTexture = std::make_shared<ImageTexture>( "images/earthmap.jpg" );
    auto earthSurface = std::make_shared<Lambertian>( earthTexture );
    auto globe        = std::make_shared<Sphere>( Point3(), 2, earthSurface );

    camera.aspectRatio     = 16.0 / 9.0;
    camera.imageWidth      = 400u;
    camera.samplesPerPixel = 100u;
    camera.maxDepth        = 50u;
    camera.backGround      = Color( 0.70, 0.80, 1.00 );

    camera.vfov     = 20.0;
    camera.lookFrom = Point3( 0, 0, 12 );
    camera.lookAt   = Point3( 0, 0, 0 );
    camera.vup      = Vec3( 0, 1, 0 );

    camera.defocusAngle = 0.0;

    camera.Render( HittableList( globe ), image );
    image.Save( "renders/earth.png" );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static void PerlinSpheres( Camera &camera, rtImage &image )
{
    HittableList world;

    auto perText = std::make_shared<NoiseTexture>( 4 );
    world.Add(
        std::make_shared<Sphere>( Point3( 0, -1000, 0 ),
                                  1000,
                                  std::make_shared<Lambertian>( perText ) ) );
    world.Add( std::make_shared<Sphere>(
        Point3( 0, 2, 0 ), 2, std::make_shared<Lambertian>( perText ) ) );

    camera.aspectRatio     = 16.0 / 9.0;
    camera.imageWidth      = 400u;
    camera.samplesPerPixel = 100u;
    camera.maxDepth        = 50u;
    camera.backGround      = Color( 0.70, 0.80, 1.00 );

    camera.vfov     = 20.0;
    camera.lookFrom = Point3( 13, 2, 3 );
    camera.lookAt   = Point3( 0, 0, 0 );
    camera.vup      = Vec3( 0, 1, 0 );

    camera.defocusAngle = 0.0;

    camera.Render( HittableList( world ), image );
    image.Save( "renders/perlin.png" );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static void Quads( Camera &camera, rtImage &image )
{
    HittableList world;

    // Materials
    auto leftRed     = std::make_shared<Lambertian>( Color( 1.0, 0.2, 0.2 ) );
    auto backGreen   = std::make_shared<Lambertian>( Color( 0.2, 1.0, 0.2 ) );
    auto rightBlue   = std::make_shared<Lambertian>( Color( 0.2, 0.2, 1.0 ) );
    auto upperOrange = std::make_shared<Lambertian>( Color( 1.0, 0.5, 0.0 ) );
    auto lowerTeal   = std::make_shared<Lambertian>( Color( 0.2, 0.8, 0.8 ) );

    // Quads
    world.Add( std::make_shared<Quad>(
        Point3( -3, -2, 5 ), Vec3( 0, 0, -4 ), Vec3( 0, 4, 0 ), leftRed ) );
    world.Add( std::make_shared<Quad>(
        Point3( -2, -2, 0 ), Vec3( 4, 0, 0 ), Vec3( 0, 4, 0 ), backGreen ) );
    world.Add( std::make_shared<Quad>(
        Point3( 3, -2, 1 ), Vec3( 0, 0, 4 ), Vec3( 0, 4, 0 ), rightBlue ) );
    world.Add( std::make_shared<Quad>(
        Point3( -2, 3, 1 ), Vec3( 4, 0, 0 ), Vec3( 0, 0, 4 ), upperOrange ) );
    world.Add( std::make_shared<Quad>(
        Point3( -2, -3, 5 ), Vec3( 4, 0, 0 ), Vec3( 0, 0, -4 ), lowerTeal ) );

    camera.aspectRatio     = 1.0;
    camera.imageWidth      = 400;
    camera.samplesPerPixel = 100;
    camera.maxDepth        = 50;
    camera.backGround      = Color( 0.70, 0.80, 1.00 );

    camera.vfov     = 80;
    camera.lookFrom = Point3( 0, 0, 9 );
    camera.lookAt   = Point3( 0, 0, 0 );
    camera.vup      = Vec3( 0, 1, 0 );

    camera.defocusAngle = 0;

    camera.Render( HittableList( world ), image );
    image.Save( "renders/quads.png" );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static void SimpleLight( Camera &camera, rtImage &image )
{
    HittableList world;

    // Materials
    auto pertext = std::make_shared<NoiseTexture>( 4 );
    world.Add(
        std::make_shared<Sphere>( Point3( 0, -1000, 0 ),
                                  1000,
                                  std::make_shared<Lambertian>( pertext ) ) );
    world.Add( std::make_shared<Sphere>(
        Point3( 0, 2, 0 ), 2, std::make_shared<Lambertian>( pertext ) ) );

    auto diffLight = std::make_shared<DiffuseLight>( Color( 4, 4, 4 ) );
    world.Add( std::make_shared<Sphere>( Point3( 0, 7, 0 ), 2, diffLight ) );
    world.Add( std::make_shared<Quad>(
        Point3( 3, 1, -2 ), Vec3( 2, 0, 0 ), Vec3( 0, 2, 0 ), diffLight ) );

    camera.aspectRatio     = 16.0 / 9.0;
    camera.imageWidth      = 400;
    camera.samplesPerPixel = 100;
    camera.maxDepth        = 50;
    camera.backGround      = Color( 0, 0, 0 );

    camera.vfov     = 20;
    camera.lookFrom = Point3( 26, 3, 6 );
    camera.lookAt   = Point3( 0, 2, 0 );
    camera.vup      = Vec3( 0, 1, 0 );

    camera.defocusAngle = 0;

    camera.Render( HittableList( world ), image );
    image.Save( "renders/light.png" );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static void CornellBox( Camera &camera, rtImage &image )
{
    HittableList world;

    auto red   = std::make_shared<Lambertian>( Color( 0.65, 0.05, 0.05 ) );
    auto white = std::make_shared<Lambertian>( Color( 0.73, 0.73, 0.73 ) );
    auto green = std::make_shared<Lambertian>( Color( 0.12, 0.45, 0.15 ) );
    auto light = std::make_shared<DiffuseLight>( Color( 15, 15, 15 ) );

    world.Add( std::make_shared<Quad>(
        Point3( 555, 0, 0 ), Vec3( 0, 555, 0 ), Vec3( 0, 0, 555 ), green ) );
    world.Add( std::make_shared<Quad>(
        Point3( 0, 0, 0 ), Vec3( 0, 555, 0 ), Vec3( 0, 0, 555 ), red ) );
    world.Add( std::make_shared<Quad>( Point3( 343, 554, 332 ),
                                       Vec3( -130, 0, 0 ),
                                       Vec3( 0, 0, -105 ),
                                       light ) );
    world.Add( std::make_shared<Quad>(
        Point3( 0, 0, 0 ), Vec3( 555, 0, 0 ), Vec3( 0, 0, 555 ), white ) );
    world.Add( std::make_shared<Quad>( Point3( 555, 555, 555 ),
                                       Vec3( -555, 0, 0 ),
                                       Vec3( 0, 0, -555 ),
                                       white ) );
    world.Add( std::make_shared<Quad>(
        Point3( 0, 0, 555 ), Vec3( 555, 0, 0 ), Vec3( 0, 555, 0 ), white ) );

    std::shared_ptr<Hittable> box1 =
        Box( Point3( 0, 0, 0 ), Point3( 165, 330, 165 ), white );
    box1 = std::make_shared<Rotate_Y>( box1, 15 );
    box1 = std::make_shared<Translate>( box1, Vec3( 265, 0, 295 ) );
    world.Add( box1 );

    std::shared_ptr<Hittable> box2 =
        Box( Point3( 0, 0, 0 ), Point3( 165, 165, 165 ), white );
    box2 = std::make_shared<Rotate_Y>( box2, -18 );
    box2 = std::make_shared<Translate>( box2, Vec3( 130, 0, 65 ) );
    world.Add( box2 );

    camera.aspectRatio     = 1.0;
    camera.imageWidth      = 300;
    camera.samplesPerPixel = 200;
    camera.maxDepth        = 50;
    camera.backGround      = Color( 0, 0, 0 );

    camera.vfov     = 40;
    camera.lookFrom = Point3( 278, 278, -800 );
    camera.lookAt   = Point3( 278, 278, 0 );
    camera.vup      = Vec3( 0, 1, 0 );

    camera.defocusAngle = 0;

    camera.Render( HittableList( world ), image );
    image.Save( "renders/CornellBox.png" );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static void CornellSmoke( Camera &camera, rtImage &image )
{
    HittableList world;

    auto red   = std::make_shared<Lambertian>( Color( .65, .05, .05 ) );
    auto white = std::make_shared<Lambertian>( Color( .73, .73, .73 ) );
    auto green = std::make_shared<Lambertian>( Color( .12, .45, .15 ) );
    auto light = std::make_shared<DiffuseLight>( Color( 7, 7, 7 ) );

    world.Add( std::make_shared<Quad>(
        Point3( 555, 0, 0 ), Vec3( 0, 555, 0 ), Vec3( 0, 0, 555 ), green ) );
    world.Add( std::make_shared<Quad>(
        Point3( 0, 0, 0 ), Vec3( 0, 555, 0 ), Vec3( 0, 0, 555 ), red ) );
    world.Add( std::make_shared<Quad>( Point3( 113, 554, 127 ),
                                       Vec3( 330, 0, 0 ),
                                       Vec3( 0, 0, 305 ),
                                       light ) );
    world.Add( std::make_shared<Quad>(
        Point3( 0, 555, 0 ), Vec3( 555, 0, 0 ), Vec3( 0, 0, 555 ), white ) );
    world.Add( std::make_shared<Quad>(
        Point3( 0, 0, 0 ), Vec3( 555, 0, 0 ), Vec3( 0, 0, 555 ), white ) );
    world.Add( std::make_shared<Quad>(
        Point3( 0, 0, 555 ), Vec3( 555, 0, 0 ), Vec3( 0, 555, 0 ), white ) );

    std::shared_ptr<Hittable> box1 =
        Box( Point3( 0, 0, 0 ), Point3( 165, 330, 165 ), white );
    box1 = std::make_shared<Rotate_Y>( box1, 15 );
    box1 = std::make_shared<Translate>( box1, Vec3( 265, 0, 295 ) );

    std::shared_ptr<Hittable> box2 =
        Box( Point3( 0, 0, 0 ), Point3( 165, 165, 165 ), white );
    box2 = std::make_shared<Rotate_Y>( box2, -18 );
    box2 = std::make_shared<Translate>( box2, Vec3( 130, 0, 65 ) );

    world.Add(
        std::make_shared<ConstantMedium>( box1, 0.01, Color( 0, 0, 0 ) ) );
    world.Add(
        std::make_shared<ConstantMedium>( box2, 0.01, Color( 1, 1, 1 ) ) );

    camera.aspectRatio     = 1.0;
    camera.imageWidth      = 600;
    camera.samplesPerPixel = 200;
    camera.maxDepth        = 50;
    camera.backGround      = Color( 0, 0, 0 );

    camera.vfov     = 40;
    camera.lookFrom = Point3( 278, 278, -800 );
    camera.lookAt   = Point3( 278, 278, 0 );
    camera.vup      = Vec3( 0, 1, 0 );

    camera.defocusAngle = 0;

    camera.Render( HittableList( world ), image );
    image.Save( "renders/CornellSmoke.png" );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static void FinalSceneBook2( Camera  &camera,
                             rtImage &image,
                             int      imageWidth,
                             int      samplesPerPixel,
                             int      maxDepth )
{
    HittableList boxes1;
    auto ground = std::make_shared<Lambertian>( Color( 0.48, 0.83, 0.53 ) );

    int boxesPerSide = 20;
    for ( int i = 0; i < boxesPerSide; i++ )
    {
        for ( int j = 0; j < boxesPerSide; j++ )
        {
            auto w  = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = RandomDouble( 1, 101 );
            auto z1 = z0 + w;

            boxes1.Add(
                Box( Point3( x0, y0, z0 ), Point3( x1, y1, z1 ), ground ) );
        }
    }

    HittableList world;

    world.Add( std::make_shared<BVHNode>( boxes1 ) );

    auto light = std::make_shared<DiffuseLight>( Color( 7, 7, 7 ) );
    world.Add( std::make_shared<Quad>( Point3( 123, 554, 147 ),
                                       Vec3( 300, 0, 0 ),
                                       Vec3( 0, 0, 265 ),
                                       light ) );

    auto center1 = Point3( 400, 400, 200 );
    auto center2 = center1 + Vec3( 30, 0, 0 );
    auto Sphere_material =
        std::make_shared<Lambertian>( Color( 0.7, 0.3, 0.1 ) );
    world.Add(
        std::make_shared<Sphere>( center1, center2, 50, Sphere_material ) );

    world.Add( std::make_shared<Sphere>(
        Point3( 260, 150, 45 ), 50, std::make_shared<Dielectric>( 1.5 ) ) );
    world.Add( std::make_shared<Sphere>(
        Point3( 0, 150, 145 ),
        50,
        std::make_shared<Metal>( Color( 0.8, 0.8, 0.9 ), 1.0 ) ) );

    auto boundary = std::make_shared<Sphere>(
        Point3( 360, 150, 145 ), 70, std::make_shared<Dielectric>( 1.5 ) );
    world.Add( boundary );
    world.Add( std::make_shared<ConstantMedium>(
        boundary, 0.2, Color( 0.2, 0.4, 0.9 ) ) );
    boundary = std::make_shared<Sphere>(
        Point3( 0, 0, 0 ), 5000, std::make_shared<Dielectric>( 1.5 ) );
    world.Add(
        std::make_shared<ConstantMedium>( boundary, .0001, Color( 1, 1, 1 ) ) );

    auto emat = std::make_shared<Lambertian>(
        std::make_shared<ImageTexture>( "images/earthmap.jpg" ) );
    world.Add( std::make_shared<Sphere>( Point3( 400, 200, 400 ), 100, emat ) );
    auto pertext = std::make_shared<NoiseTexture>( 0.2 );
    world.Add(
        std::make_shared<Sphere>( Point3( 220, 280, 300 ),
                                  80,
                                  std::make_shared<Lambertian>( pertext ) ) );

    HittableList boxes2;
    auto         white = std::make_shared<Lambertian>( Color( .73, .73, .73 ) );
    int          ns    = 1000;
    for ( int j = 0; j < ns; j++ )
    {
        boxes2.Add(
            std::make_shared<Sphere>( Point3::Random( 0, 165 ), 10, white ) );
    }

    world.Add( std::make_shared<Translate>(
        std::make_shared<Rotate_Y>( std::make_shared<BVHNode>( boxes2 ), 15 ),
        Vec3( -100, 270, 395 ) ) );

    camera.aspectRatio     = 1.0;
    camera.imageWidth      = imageWidth;
    camera.samplesPerPixel = samplesPerPixel;
    camera.maxDepth        = maxDepth;
    camera.backGround      = Color( 0, 0, 0 );

    camera.vfov     = 40;
    camera.lookFrom = Point3( 478, 278, -600 );
    camera.lookAt   = Point3( 278, 278, 0 );
    camera.vup      = Vec3( 0, 1, 0 );

    camera.defocusAngle = 0;

    camera.Render( HittableList( world ), image );
    image.Save( "renders/FinalSceneBook2.png" );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class RaytracerGui : public tgrApplication
{
public:
    RaytracerGui( const tgrWindowProperties &wprops = tgrWindowProperties(),
                  bool                       initDearImGui = true );

    virtual ~RaytracerGui();

    virtual void Update() override;

private:
    void                  BeginRenderingScene( int index );
    std::function<void()> RenderSceneFn() const;
    struct RenderTask
    {
        RenderTask()  = default;
        ~RenderTask() = default;
        void operator()()
        {
        }

        // std::atomic_int8_t state = 1; // abort = 0, running = 1, finished = 2
    };

    int         p_selectedScene    = 1;
    int         p_highlightedScene = -1;
    int         p_renderingScene   = -1;
    Camera      p_camera;
    rtImage     p_image;
    std::thread p_renderThread;

    tgrCamera                   p_eye;
    std::unique_ptr<tgrTexture> _texture;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
RaytracerGui::RaytracerGui( const tgrWindowProperties &wprops,
                            bool                       initDearImGui )
    : tgrApplication( wprops, initDearImGui )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
RaytracerGui::~RaytracerGui()
{
    p_camera.AbortRender();
    // wait for render thread to finish
    if ( p_renderThread.joinable() )
        p_renderThread.join();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void RaytracerGui::Update()
{
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

    // ImGui render
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    int width, height;
    GetWindowSize( width, height );
    width *= 0.75;
    height *= 0.75;
    std::vector<const char *> scenes{ "MaterialTest",
                                      "BookCoverImage",
                                      "Earth",
                                      "PerlinSpheres",
                                      "Quads",
                                      "SimpleLight",
                                      "CornellBox",
                                      "CornellSmoke",
                                      "FinalSceneBook2" };

    ImGui::DockSpaceOverViewport();
    ImGui::Begin( "Scenes", nullptr, ImGuiWindowFlags_NoDecoration );
    if ( _texture )
    {
        static auto lastDraw = std::chrono::high_resolution_clock::now();
        auto        currDraw = std::chrono::high_resolution_clock::now();
        int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                          currDraw - lastDraw )
                          .count();
        if ( elapsed > 25 )
        {
            _texture->Update(
                p_image.PixelData(), p_image.Width(), p_image.Height(), 3 );
            lastDraw = currDraw;
        }

        ImGui::Begin( "Render" );
        ImGui::Image(
                _texture->GetID(),
                ImVec2( width,
                    _texture->GetHeight() *
                    ( ( 1.0f * width ) / _texture->GetWidth() ) ) );
        ImGui::End();
    }

    if ( ImGui::BeginListBox(
             "##Scenes",
             ImVec2( -FLT_MIN, (scenes.size() + 1) * ImGui::GetTextLineHeightWithSpacing() ) ) )
    {
        for ( int n = 0; n < static_cast<int>( scenes.size() ); n++ )
        {
            bool                 isSelected = ( p_selectedScene == n );
            ImGuiSelectableFlags flags = ( p_highlightedScene == n )
                                             ? ImGuiSelectableFlags_Highlight
                                             : 0;
            if ( ImGui::Selectable( scenes[n], &isSelected, flags ) && isSelected )
                p_selectedScene = n;
        }
        ImGui::EndListBox();
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

    if ( p_renderingScene != p_selectedScene )
    {
        p_renderingScene = p_selectedScene;
        p_camera.AbortRender();
        // wait for render thread to finish
        if ( p_renderThread.joinable() )
            p_renderThread.join();

        _texture = std::make_unique<tgrTexture>(
            p_image.PixelData(), p_image.Width(), p_image.Height(), 3 );

        p_camera.Reset();
        p_renderThread =
            std::thread( [&]() { BeginRenderingScene( p_renderingScene ); } );
    }

    tgrApplication::Update();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void RaytracerGui::BeginRenderingScene( int index )
{
    switch ( index )
    {
    case 0:
        MaterialTest( p_camera, p_image );
        break;
    case 1:
        BookCoverImage( p_camera, p_image, false );
        break;
    case 2:
        Earth( p_camera, p_image );
        break;
    case 3:
        PerlinSpheres( p_camera, p_image );
        break;
    case 4:
        Quads( p_camera, p_image );
        break;
    case 5:
        SimpleLight( p_camera, p_image );
        break;
    case 6:
        CornellBox( p_camera, p_image );
        break;
    case 7:
        CornellSmoke( p_camera, p_image );
        break;
    case 8:
        FinalSceneBook2( p_camera, p_image, 400, 250, 4 );
        break;
    default:
        break;
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
int main( int argc, const char *argv[] )
{
    tgrWindowProperties wprops;
    RaytracerGui        ui( wprops, true );
    ui.Run();

    return 0u;
}
