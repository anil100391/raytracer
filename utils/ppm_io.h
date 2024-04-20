#include <string>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace utils
{

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool WritePPM( const std::string& file,
               unsigned int w,
               unsigned int h,
               const uint8_t* image );
}