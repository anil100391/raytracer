#include <memory>

#include "ppm_io.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool utils::WritePPM( const std::string& file,
                      unsigned int w,
                      unsigned int h,
                      const uint8_t* image )
{
    auto fp = std::unique_ptr<FILE, decltype(&fclose)>( fopen( file.c_str(), "w" ), &fclose );
    if ( !fp )
    {
        return false;
    }

    auto fptr = fp.get();
    fprintf( fptr, "P3\n%d %d\n255\n", w, h );

    int cnt = 0u;
    for ( auto ii = 0u; ii < h; ++ii )
    {
        for ( auto jj = 0u; jj < w; ++jj )
        {
            fprintf( fptr, "%d %d %d ", image[cnt++], image[cnt++], image[cnt++] );
        }
        fprintf( fptr, "\n" );
    }

    return true;
}
