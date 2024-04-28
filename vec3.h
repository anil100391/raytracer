#ifndef _VEC3_H_
#define _VEC3_H_

#include <ostream>
#include <cassert>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
template <typename T>
class vec3
{
public:

    vec3() = default;
    vec3( T a, T b, T c ) : x( a ), y( b ), z( c ) {}

    T& operator[]( unsigned int i ) const
    {
        switch ( i )
        {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        }

        assert( false );
        return x;
    }

    vec3 operator+( const vec3& other ) const
    {
        return vec3{ x + other.x, y + other.y, z + other.z };
    }

    vec3 operator-( const vec3& other ) const
    {
        return vec3{ x - other.x, y - other.y, z - other.z };
    }

    vec3& operator+=( const vec3& other )
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    vec3 operator-=( const vec3& other )
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    vec3 operator*( T s ) const
    {
        return vec3{ x * s, y * s, z * s };
    }

    vec3 operator/( T s ) const
    {
        return vec3{ x / scale, y / scale, z / scale };
    }

    vec3& operator*=( T s )
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    vec3& operator/=( T s )
    {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    T operator %( const vec3& other ) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    vec3 operator *( const vec3& other ) const
    {
        return vec3{ y * other.z - other.y * z,
                     other.x * z - x * other.z,
                     x * other.y - other.x * y };
    }

    T len2() const
    {
        return x * x + y * y + z * z;
    }

    T len() const
    {
        static_assert(std::is_floating_point_v<T>);
        return std::sqrt( len2() );
    }

    std::enable_if_t<std::is_floating_point_v<T>, T> normalize()
    {
        static_assert(std::is_floating_point_v<T>);
        auto length = len();
        if ( length > 0 )
        {
            (*this) /= len();
        }

        return length;
    }

    T x{ 0 }, y{ 0 }, z{ 0 };
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
template<typename T>
std::ostream& operator<<( std::ostream& o, const vec3<T>& v )
{
    o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return o;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
using vec3f = vec3<float>;
using vec3d = vec3<double>;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
template<typename T> using point3 = vec3<T>;
using point3f = vec3f;
using point3d = vec3d;

#endif // _VEC3_H_
