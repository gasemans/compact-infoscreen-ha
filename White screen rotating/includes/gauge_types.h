#pragma once

#include "esphome.h"
#include "colors.h"

namespace ui {

///////////////////////////////////////////////////////////////////////////////
// Gauge Types
///////////////////////////////////////////////////////////////////////////////

enum class GaugeType
{
    BATTERY,
    SALT,
    TEMPERATURE,
    WEATHER,
    GENERIC
};

///////////////////////////////////////////////////////////////////////////////
// Gauge Style
///////////////////////////////////////////////////////////////////////////////

struct GaugeStyle
{
    // Geometry
    int radius = 90;
    int thickness = 14;

    // Sweep
    int start_angle = 135;
    int end_angle = 405;

    // Colors
    Color background = RING_BACKGROUND;
    Color foreground = GREEN;
    Color text = TEXT;
    Color secondary = TEXT_SECONDARY;

    // Features
    bool rounded = true;
    bool show_shadow = false;
    bool show_ticks = true;
    bool show_value = true;
    bool show_unit = true;
    bool show_title = true;

    // Tick configuration
    int major_ticks = 10;
    int minor_ticks = 5;

    int major_tick_length = 8;
    int minor_tick_length = 4;

    int major_tick_width = 2;
    int minor_tick_width = 1;
};

///////////////////////////////////////////////////////////////////////////////
// Gauge Data
///////////////////////////////////////////////////////////////////////////////

struct GaugeData
{
    float value = 0.0f;
    float minimum = 0.0f;
    float maximum = 100.0f;

    const char *title = "";
    const char *subtitle = "";
    const char *unit = "%";
    const char *icon = "";

    // Optional fixed color
    Color color = GREEN;

    // Automatically color based on percentage
    bool auto_color = true;
};

///////////////////////////////////////////////////////////////////////////////
// Predefined styles
///////////////////////////////////////////////////////////////////////////////

inline GaugeStyle battery_style()
{
    GaugeStyle style;
    style.foreground = GREEN;
    style.show_ticks = true;
    return style;
}

inline GaugeStyle salt_style()
{
    GaugeStyle style;
    style.foreground = CYAN;
    style.show_ticks = true;
    return style;
}

inline GaugeStyle temperature_style()
{
    GaugeStyle style;
    style.foreground = ORANGE;
    style.show_ticks = true;
    return style;
}

inline GaugeStyle weather_style()
{
    GaugeStyle style;
    style.foreground = BLUE;
    style.show_ticks = false;
    return style;
}

inline GaugeStyle generic_style()
{
    GaugeStyle style;
    return style;
}

///////////////////////////////////////////////////////////////////////////////
// Helper
///////////////////////////////////////////////////////////////////////////////

inline GaugeStyle style_for_type(GaugeType type)
{
    switch (type)
    {
        case GaugeType::BATTERY:
            return battery_style();

        case GaugeType::SALT:
            return salt_style();

        case GaugeType::TEMPERATURE:
            return temperature_style();

        case GaugeType::WEATHER:
            return weather_style();

        default:
            return generic_style();
    }
}

} // namespace ui