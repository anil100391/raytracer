#include <ppmio.h>
#include <fstream>
#include <rtSTBImage.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool rtPPMio::WritePPM( const std::filesystem::path &file,
                        const rtImage &image )
{
    std::ofstream f( file );
    if ( !f.is_open() )
    {
        return false;
    }

    auto width = image.Width();
    auto height = image.Height();
    auto buffer = image.PixelData();
    f << "P3\n" << width << " " << height << "\n255\n";

    for ( auto ii = 0u; ii < height; ++ii )
    {
        for ( auto jj = 0u; jj < width; ++jj )
        {
            f << (int)buffer[0] << " " << (int)buffer[1] << " " << (int)buffer[2] << "\n";
            buffer += 3u;
        }
    }

    return true;
}
