#pragma once

#include "esphome.h"
#include "colors.h"

#include <cmath>
#include <algorithm>

namespace ui {

///////////////////////////////////////////////////////////////////////////////
// Algemene numerieke functies
///////////////////////////////////////////////////////////////////////////////

inline float clamp(float value, float minimum, float maximum)
{
    if (!std::isfinite(value))
        return minimum;

    if (value < minimum)
        return minimum;

    if (value > maximum)
        return maximum;

    return value;
}

inline int clamp_int(int value, int minimum, int maximum)
{
    if (value < minimum)
        return minimum;

    if (value > maximum)
        return maximum;

    return value;
}

inline float average(float a, float b)
{
    return (a + b) * 0.5f;
}

inline float percentage(float value, float minimum, float maximum)
{
    if (maximum <= minimum)
        return 0.0f;

    value = clamp(value, minimum, maximum);

    return ((value - minimum) * 100.0f) / (maximum - minimum);
}

inline float mapf(float value,
                  float in_min,
                  float in_max,
                  float out_min,
                  float out_max)
{
    if (fabsf(in_max - in_min) < 0.00001f)
        return out_min;

    return (value - in_min) *
           (out_max - out_min) /
           (in_max - in_min) +
           out_min;
}

///////////////////////////////////////////////////////////////////////////////
// Controle functies
///////////////////////////////////////////////////////////////////////////////

inline bool is_valid(float value)
{
    return !std::isnan(value) && !std::isinf(value);
}

inline float sanitize(float value, float fallback = 0.0f)
{
    if (!is_valid(value))
        return fallback;

    return value;
}

///////////////////////////////////////////////////////////////////////////////
// Afronden
///////////////////////////////////////////////////////////////////////////////

inline int round_int(float value)
{
    return static_cast<int>(std::lround(value));
}

inline int floor_int(float value)
{
    return static_cast<int>(std::floor(value));
}

inline int ceil_int(float value)
{
    return static_cast<int>(std::ceil(value));
}

///////////////////////////////////////////////////////////////////////////////
// Kleuren
///////////////////////////////////////////////////////////////////////////////

inline Color battery_color(float percentage)
{
    return percentage_color(percentage);
}

inline Color salt_color(float percentage)
{
    return percentage_color(percentage);
}

inline Color temperature_color(float temperature)
{
    if (temperature < 15.0f)
        return BLUE;

    if (temperature < 22.0f)
        return GREEN;

    if (temperature < 28.0f)
        return ORANGE;

    return RED;
}

///////////////////////////////////////////////////////////////////////////////
// Tekst helpers
///////////////////////////////////////////////////////////////////////////////

inline const char *weekday(uint8_t day)
{
    static const char *days[] =
    {
        "Zondag",
        "Maandag",
        "Dinsdag",
        "Woensdag",
        "Donderdag",
        "Vrijdag",
        "Zaterdag"
    };

    if (day > 6)
        return "";

    return days[day];
}

inline const char *month(uint8_t month)
{
    static const char *months[] =
    {
        "",
        "Januari",
        "Februari",
        "Maart",
        "April",
        "Mei",
        "Juni",
        "Juli",
        "Augustus",
        "September",
        "Oktober",
        "November",
        "December"
    };

    if (month > 12)
        return "";

    return months[month];
}

///////////////////////////////////////////////////////////////////////////////
// Status helpers
///////////////////////////////////////////////////////////////////////////////

inline const char *battery_state(float percentage)
{
    if (percentage >= 80.0f)
        return "Excellent";

    if (percentage >= 60.0f)
        return "Goed";

    if (percentage >= 30.0f)
        return "Normaal";

    if (percentage >= 15.0f)
        return "Laag";

    return "Kritiek";
}

///////////////////////////////////////////////////////////////////////////////
// Hoeken
///////////////////////////////////////////////////////////////////////////////

constexpr float DEG_TO_RAD = 0.017453292519943295f;
constexpr float RAD_TO_DEG = 57.29577951308232f;

inline float deg2rad(float degrees)
{
    return degrees * DEG_TO_RAD;
}

inline float rad2deg(float radians)
{
    return radians * RAD_TO_DEG;
}

///////////////////////////////////////////////////////////////////////////////
// Percentage formatter
///////////////////////////////////////////////////////////////////////////////

inline int percent_width(float percentage, int maximum_width)
{
    percentage = clamp(percentage, 0.0f, 100.0f);

    return round_int(
        (percentage / 100.0f) *
        static_cast<float>(maximum_width));
}

///////////////////////////////////////////////////////////////////////////////
// Display helpers
///////////////////////////////////////////////////////////////////////////////

inline int center_x(int width)
{
    return width / 2;
}

inline int center_y(int height)
{
    return height / 2;
}

///////////////////////////////////////////////////////////////////////////////
// Waarden vergelijken
///////////////////////////////////////////////////////////////////////////////

inline float minimum(float a, float b)
{
    return std::min(a, b);
}

inline float maximum(float a, float b)
{
    return std::max(a, b);
}

} // namespace ui