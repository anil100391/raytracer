#pragma once

#include <cmath>
#include <concepts>
#include <iostream>
#include <type_traits>

#include <rtDefs.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
template <typename T>
concept is_numeric = std::is_integral_v<T> || std::is_floating_point_v<T>;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Vec3
{
public:

    Vec3() = default;
    ~Vec3() = default;

    template<typename T> requires is_numeric<T>
    Vec3( T a, T b, T c )
    {
        e[0] = a; e[1] = b; e[2] = c;
    }

    template<typename T> requires is_numeric<T>
    Vec3( const T *p )
    {
        e[0] = p[0]; e[1] = p[1]; e[2] = p[2];
    }

    [[nodiscard]] double x() const noexcept { return e[0]; }
    [[nodiscard]] double y() const noexcept { return e[1]; }
    [[nodiscard]] double z() const noexcept { return e[2]; }

    [[nodiscard]] Vec3 operator-() const { return Vec3( -e[0], -e[1], -e[2] ); }
    [[nodiscard]] double operator[]( int i ) const { return e[i]; }
    [[nodiscard]] double& operator[]( int i ) { return e[i]; }

    Vec3& operator+=( const Vec3 &v )
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vec3& operator*=( double t )
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vec3& operator/=( double t )
    {
        return *this *= 1 / t;
    }

    [[nodiscard]] double Length() const
    {
        return std::sqrt( Len2() );
    }

    [[nodiscard]] double Len2() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    [[nodiscard]] bool NearZero() const
    {
        auto s = 1e-8;
        return (std::fabs( e[0] ) < s) && (std::fabs( e[1] ) < s) && (std::fabs( e[2] ) < s);
    }

    [[nodiscard]] static Vec3 Random()
    {
        return Vec3( RandomDouble(), RandomDouble(), RandomDouble() );
    }

    [[nodiscard]] static Vec3 Random( double min, double max )
    {
        return Vec3( RandomDouble( min, max ),
                     RandomDouble( min, max ),
                     RandomDouble( min, max ) );
    }

    double e[3] = {0.0, 0.0, 0.0};
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
using Point3 = Vec3;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline std::ostream& operator<<( std::ostream &out, const Vec3 &v )
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline Vec3 operator+( const Vec3 &u, const Vec3 &v )
{
    return Vec3( u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2] );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline Vec3 operator-( const Vec3 &u, const Vec3 &v )
{
    return Vec3( u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2] );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline Vec3 operator*( const Vec3 &u, const Vec3 &v )
{
    return Vec3( u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2] );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline Vec3 operator*( double t, const Vec3 &v )
{
    return Vec3( t * v.e[0], t * v.e[1], t * v.e[2] );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline Vec3 operator*( const Vec3 &v, double t )
{
    return t * v;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline Vec3 operator/( const Vec3 &v, double t )
{
    return (1 / t) * v;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline double Dot( const Vec3 &u, const Vec3 &v )
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline Vec3 Cross( const Vec3 &u, const Vec3 &v )
{
    return Vec3( u.e[1] * v.e[2] - u.e[2] * v.e[1],
                 u.e[2] * v.e[0] - u.e[0] * v.e[2],
                 u.e[0] * v.e[1] - u.e[1] * v.e[0] );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline Vec3 Normalize( const Vec3 &v )
{
    return v / v.Length();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline Vec3 RandomInUnitDisk()
{
    while ( true )
    {
        auto p = Vec3( RandomDouble( -1, 1 ), RandomDouble( -1, 1 ), 0.0 );
        if ( p.Len2() < 1 )
            return p;
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline Vec3 RandomUnitVector()
{
    while ( true )
    {
        auto p = Vec3::Random( -1, 1 );
        auto len2 = p.Len2();
        if ( 1e-160 < len2 && len2 <= 1 )
            return p / std::sqrt( len2 );
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline Vec3 RandomOnHemisphere( const Vec3 &normal )
{
    Vec3 onUnitSphere = RandomUnitVector();
    return (Dot( onUnitSphere, normal ) > 0.0) ? onUnitSphere : -onUnitSphere;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline Vec3 Reflect( const Vec3 &v, const Vec3 &normal )
{
    return v - 2 * Dot( v, normal ) * normal;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline Vec3 Refract( const Vec3 &uv, const Vec3 &n, double etaiOverEtat )
{
    auto cosTheta = std::fmin( Dot( -uv, n ), 1.0 );
    Vec3 rOutPerp = etaiOverEtat * (uv + cosTheta * n);
    Vec3 rOutPar = -std::sqrt( std::fabs( 1.0 - rOutPerp.Len2() ) ) * n;
    return rOutPerp + rOutPar;
}
