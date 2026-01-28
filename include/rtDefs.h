#pragma once

#include <random>
#include <limits>
#include <numbers>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
const double infinity = std::numeric_limits<double>::infinity();
const double pi = std::numbers::pi_v<double>;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline double DegreesToRadians( double degrees )
{
    return degrees * pi / 180.0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline double RandomDouble()
{
    static std::uniform_real_distribution<double> distribution( 0.0, 1.0 );
    static std::mt19937 generator;
    return distribution( generator );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline double RandomDouble(double min, double max)
{
    return min + (max - min) * RandomDouble();
}
