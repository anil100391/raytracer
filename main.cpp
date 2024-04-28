#include <vector>
#include <iostream>
#include "ray.h"
#include "utils/ppm_io.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
	/*
	std::vector<uint8_t> image{ 255, 0, 0, 0, 255, 0, 0, 0, 255,
	                            255, 255, 0, 0, 255, 255, 255, 0, 255 };
	if ( !utils::WritePPM( "image.ppm", 3, 2, image.data() ) )
	{
		return -1;
	}*/

	vec3f x{ 1.0f, 0.0f, 0.0f };
	vec3f y{ 0.0f, 1.0f, 0.0f };
	rayf r{ x, y };

	x /= 2.0f;
	std::cout << "x: " << x << " y: " << y << "\n";
	std::cout << r << "\n";
	return 0u;
}