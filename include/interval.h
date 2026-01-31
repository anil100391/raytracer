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
    Interval(const Interval &a, const Interval &b)
    {
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }

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

    [[nodiscard]] Interval Expand( double delta ) const noexcept
    {
        auto padding = delta / 2;
        return Interval( min - padding, max + padding );
    }

    static const Interval Empty;
    static const Interval Universe;
};
