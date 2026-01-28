#pragma once

#include <filesystem>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class rtPPMio
{
public:

    static bool WritePPM( const std::filesystem::path &file,
                          unsigned int width,
                          unsigned int height,
                          const uint8_t *buffer );
};
