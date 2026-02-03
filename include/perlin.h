#pragma once

#include <vec3.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Perlin
{
public:

    Perlin();

    double Noise( const Point3 &p ) const;

    double Turbulence( const Point3 &p, int depth ) const;

private:

    static void PerlinGeneratePerm( int *p );
    static void Permute( int *p, int n );
    static double TrilinearInterp( const Vec3 c[2][2][2], double u, double v, double w );

    static const int s_pointCount = 256;
    Vec3            _randVec[s_pointCount];
    int               _permX[s_pointCount];
    int               _permY[s_pointCount];
    int               _permZ[s_pointCount];
};
