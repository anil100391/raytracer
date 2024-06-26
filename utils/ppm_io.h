#ifndef _UTILS_H_
#define _UTILS_H_

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

#endif // _UTILS_H_