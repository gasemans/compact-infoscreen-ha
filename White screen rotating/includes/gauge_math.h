#pragma once

#include <cmath>
#include <algorithm>

#include "helpers.h"
#include "gauge_types.h"

namespace ui {

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

constexpr float PI_F = 3.14159265358979323846f;

///////////////////////////////////////////////////////////////////////////////
// Angle conversions
///////////////////////////////////////////////////////////////////////////////

inline float degrees_to_radians(float degrees)
{
    return degrees * PI_F / 180.0f;
}

inline float radians_to_degrees(float radians)
{
    return radians * 180.0f / PI_F;
}

///////////////////////////////////////////////////////////////////////////////
// Value calculations
///////////////////////////////////////////////////////////////////////////////

inline float normalized_value(float value,
                              float minimum,
                              float maximum)
{
    if (maximum <= minimum)
        return 0.0f;

    value = clamp(value, minimum, maximum);

    return (value - minimum) / (maximum - minimum);
}

inline float gauge_percentage(float value,
                              float minimum,
                              float maximum)
{
    return normalized_value(value, minimum, maximum) * 100.0f;
}

///////////////////////////////////////////////////////////////////////////////
// Angle calculations
///////////////////////////////////////////////////////////////////////////////

inline float gauge_angle(const GaugeStyle &style,
                         float value,
                         float minimum,
                         float maximum)
{
    const float fraction =
        normalized_value(value, minimum, maximum);

    return style.start_angle +
           fraction *
           (style.end_angle - style.start_angle);
}

inline float sweep_angle(const GaugeStyle &style)
{
    return style.end_angle - style.start_angle;
}

///////////////////////////////////////////////////////////////////////////////
// Segment calculations
///////////////////////////////////////////////////////////////////////////////

inline int total_segments(float sweep_degrees,
                          float degrees_per_segment)
{
    if (degrees_per_segment <= 0.0f)
        return 0;

    return static_cast<int>(
        std::round(sweep_degrees / degrees_per_segment));
}

inline int filled_segments(const GaugeStyle &style,
                           float value,
                           float minimum,
                           float maximum,
                           int segment_count)
{
    float fraction =
        normalized_value(value, minimum, maximum);

    return static_cast<int>(
        std::round(fraction * segment_count));
}

///////////////////////////////////////////////////////////////////////////////
// Geometry
///////////////////////////////////////////////////////////////////////////////

struct Point
{
    int x;
    int y;
};

inline Point polar_to_cartesian(int center_x,
                                int center_y,
                                float radius,
                                float angle_deg)
{
    float angle =
        degrees_to_radians(angle_deg - 90.0f);

    Point p;

    p.x = center_x +
          static_cast<int>(
              std::round(std::cos(angle) * radius));

    p.y = center_y +
          static_cast<int>(
              std::round(std::sin(angle) * radius));

    return p;
}

///////////////////////////////////////////////////////////////////////////////
// Tick calculations
///////////////////////////////////////////////////////////////////////////////

inline float tick_angle(const GaugeStyle &style,
                        int tick,
                        int total_ticks)
{
    if (total_ticks <= 1)
        return static_cast<float>(style.start_angle);

    return style.start_angle +
           (static_cast<float>(tick) /
            (total_ticks - 1)) *
               sweep_angle(style);
}

///////////////////////////////////////////////////////////////////////////////
// Radius helpers
///////////////////////////////////////////////////////////////////////////////

inline int inner_radius(const GaugeStyle &style)
{
    return style.radius - style.thickness;
}

inline int outer_radius(const GaugeStyle &style)
{
    return style.radius;
}

inline int middle_radius(const GaugeStyle &style)
{
    return style.radius - (style.thickness / 2);
}

///////////////////////////////////////////////////////////////////////////////
// Animation helpers
///////////////////////////////////////////////////////////////////////////////

inline float interpolate(float from,
                         float to,
                         float progress)
{
    progress = clamp(progress, 0.0f, 1.0f);

    return from +
           ((to - from) * progress);
}

///////////////////////////////////////////////////////////////////////////////
// Value helpers
///////////////////////////////////////////////////////////////////////////////

inline bool is_low(float value)
{
    return value < 20.0f;
}

inline bool is_warning(float value)
{
    return value >= 20.0f &&
           value < 50.0f;
}

inline bool is_good(float value)
{
    return value >= 50.0f;
}

///////////////////////////////////////////////////////////////////////////////
// Automatic gauge color
///////////////////////////////////////////////////////////////////////////////

inline Color gauge_color(const GaugeData &data)
{
    return percentage_color(
        gauge_percentage(
            data.value,
            data.minimum,
            data.maximum));
}

///////////////////////////////////////////////////////////////////////////////
// Text formatting
///////////////////////////////////////////////////////////////////////////////

inline int rounded_value(const GaugeData &data)
{
    return static_cast<int>(
        std::round(data.value));
}

inline float clamped_value(const GaugeData &data)
{
    return clamp(
        data.value,
        data.minimum,
        data.maximum);
}

} // namespace ui