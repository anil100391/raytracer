#pragma once

#include <rtDefs.h>
#include <algorithm>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class Interval
{
public:

    double min, max;

    Interval() : min( infinity ), max( -infinity ) {}
    Interval( double minn, double maxx ) : min( minn ), max( maxx ) {}

    [[nodiscard]] double Size() const noexcept
    {
        return max - min;
    }


    [[nodiscard]] double Contains( double x ) const noexcept
    {
        return min <= x && x <= max;
    }

    [[nodiscard]] double Surrounds( double x ) const noexcept
    {
        return min < x && x < max;
    }

    [[nodiscard]] double Clamp( double x ) const noexcept
    {
        return std::clamp( x, min, max );
    }

    static const Interval empty;
    static const Interval universe;
};
