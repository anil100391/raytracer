#include <ppmio.h>
#include <fstream>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool rtPPMio::WritePPM( const std::filesystem::path &file,
                        unsigned int width,
                        unsigned int height,
                        const uint8_t *buffer )
{
    std::ofstream f( file );
    if ( !f.is_open() )
    {
        return false;
    }

    f << "P3\n" << width << " " << height << "\n255\n";

    for ( auto ii = 0u; ii < height; ++ii )
    {
        for ( auto jj = 0u; jj < width; ++jj )
        {
            f << (int)buffer[0] << " " << (int)buffer[1] << " " << (int)buffer[2] << "\n";
            buffer += 3;
        }
    }

    return true;
}
