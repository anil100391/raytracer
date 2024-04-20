#include <vector>
#include "utils/ppm_io.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
	std::vector<uint8_t> image{ 255, 0, 0, 0, 255, 0, 0, 0, 255,
	                            255, 255, 0, 0, 255, 255, 255, 0, 255 };
	if ( !utils::WritePPM( "image.ppm", 3, 2, image.data() ) )
	{
		return -1;
	}

	return 0u;
}