#ifndef _RAY_H_
#define _RAY_H_

#include "vec3.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
template <typename T>
class ray
{
public:

    ray() = default;
    ray( const point3<T>& origin, const vec3<T>& direction )
        : o( origin ), d( direction )
    {
        d.normalize();
    }

    point3<T> at( double t ) const
    {
        return point3{ o + d * t };
    }

    const point3<T>& origin() const { return o; }
    const vec3<T>&   direction() const { return d; }

    point3<T> o{ 0, 0, 0 }; // origin
    vec3<T>   d{ 1, 0, 0 }; // direction
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
template<typename T>
std::ostream& operator<<( std::ostream& o, const ray<T>& r )
{
    o << "o: " << r.o << " d: " << r.d;
    return o;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
using rayf = ray<float>;
using rayd = ray<double>;

#endif // _RAY_H_