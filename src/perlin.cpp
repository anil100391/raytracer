#include <perlin.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
Perlin::Perlin()
{
    for ( int i = 0; i < s_pointCount; ++i )
    {
        _randVec[i] = Normalize( Vec3::Random( -1, 1 ) );
    }

    PerlinGeneratePerm( _permX );
    PerlinGeneratePerm( _permY );
    PerlinGeneratePerm( _permZ );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
double Perlin::Noise( const Point3 &p ) const
{
    auto u = p.x() - std::floor( p.x() );
    auto v = p.y() - std::floor( p.y() );
    auto w = p.z() - std::floor( p.z() );

    auto i = int( std::floor( p.x() ) );
    auto j = int( std::floor( p.y() ) );
    auto k = int( std::floor( p.z() ) );
    Vec3 c[2][2][2];

    for ( int di = 0; di < 2; ++di )
        for ( int dj = 0; dj < 2; ++dj )
            for ( int dk = 0; dk < 2; ++dk )
                c[di][dj][dk] = _randVec[
                    _permX[(i + di) & 255] ^
                    _permX[(j + dj) & 255] ^
                    _permX[(k + dk) & 255]
                ];

    return TrilinearInterp( c, u, v, w );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
double Perlin::Turbulence( const Point3 &p, int depth ) const
{
    auto accum = 0.0;
    auto tempP = p;
    auto weight = 1.0;

    for ( int i = 0; i < depth; ++i )
    {
        accum += weight * Noise( tempP );
        weight *= 0.5;
        tempP *= 2;
    }

    return std::fabs( accum );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void Perlin::PerlinGeneratePerm( int *p )
{
    for ( int i = 0; i < s_pointCount; ++i )
    {
        p[i] = i;
    }

    Permute( p, s_pointCount );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void Perlin::Permute( int *p, int n )
{
    for ( int i = n - 1; i > 0; i-- )
    {
        int target = RandomInt( 0, i );
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
double Perlin::TrilinearInterp( const Vec3 c[2][2][2], double u, double v, double w )
{
    auto uu = u * u * (3 - 2 * u);
    auto vv = v * v * (3 - 2 * v);
    auto ww = w * w * (3 - 2 * w);
    auto accum = 0.0;

    for ( int i = 0; i < 2; ++i )
        for ( int j = 0; j < 2; ++j )
            for ( int k = 0; k < 2; ++k )
            {
                Vec3 weightV( u - i, v - j, w - k );
                accum += (i * uu + (1 - i) * (1 - uu)) *
                         (j * vv + (1 - j) * (1 - vv)) *
                         (k * ww + (1 - k) * (1 - ww)) *
                         Dot( c[i][j][k], weightV );
            }

    return accum;
}
