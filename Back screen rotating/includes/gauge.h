#pragma once

#include "esphome.h"

#include "colors.h"
#include "layouts.h"
#include "helpers.h"

#include "gauge_types.h"
#include "gauge_math.h"
#include "gauge_text.h"
#include "gauge_ring.h"

namespace ui {

///////////////////////////////////////////////////////////////////////////////
// Draw complete gauge
///////////////////////////////////////////////////////////////////////////////

inline void draw_gauge(
    display::Display &it,
    int center_x,
    int center_y,
    BaseFont *title_font,
    BaseFont *value_font,
    BaseFont *subtitle_font,
    const GaugeStyle &style,
    const GaugeData &data)
{
    ///////////////////////////////////////////////////////////////////////////
    // Calculate percentage
    ///////////////////////////////////////////////////////////////////////////

    const float percentage =
        gauge_percentage(
            data.value,
            data.minimum,
            data.maximum);

    ///////////////////////////////////////////////////////////////////////////
    // Determine color
    ///////////////////////////////////////////////////////////////////////////

    Color value_color =
        data.auto_color
            ? percentage_color(percentage)
            : data.color;

    ///////////////////////////////////////////////////////////////////////////
    // Draw complete ring
    ///////////////////////////////////////////////////////////////////////////

    draw_ring(
        it,
        center_x,
        center_y,
        style,
        percentage);

    ///////////////////////////////////////////////////////////////////////////
    // Icon
    ///////////////////////////////////////////////////////////////////////////

    if (data.icon != nullptr &&
        data.icon[0] != '\0')
    {
        draw_icon(
            it,
            center_x,
            center_y - style.radius + 18,
            title_font,
            value_color,
            data.icon);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Title
    ///////////////////////////////////////////////////////////////////////////

    if (style.show_title &&
        data.title != nullptr &&
        data.title[0] != '\0')
    {
        draw_title(
            it,
            center_x,
            center_y - 56,
            title_font,            
            value_color,
            data.title);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Value
    ///////////////////////////////////////////////////////////////////////////

    if (style.show_value)
    {
        if (style.show_unit)
        {
            draw_value_unit(
                it,
                center_x,
                center_y,
                value_font,
                subtitle_font,
                value_color,
                data.value,
                data.unit);
        }
        else
        {
            draw_value(
                it,
                center_x,
                center_y,
                value_font,
                value_color,
                data.value);
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Subtitle
    ///////////////////////////////////////////////////////////////////////////

    if (data.subtitle != nullptr &&
        data.subtitle[0] != '\0')
    {
        draw_subtitle(
            it,
            center_x,
            center_y + 42,
            subtitle_font,
            style.secondary,
            data.subtitle);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Convenience overload
///////////////////////////////////////////////////////////////////////////////

inline void draw_gauge(
    display::Display &it,
    int center_x,
    int center_y,
    BaseFont *title_font,
    BaseFont *value_font,
    BaseFont *subtitle_font,
    GaugeType type,
    const GaugeData &data)
{
    draw_gauge(
        it,
        center_x,
        center_y,
        title_font,
        value_font,
        subtitle_font,
        style_for_type(type),
        data);
}

///////////////////////////////////////////////////////////////////////////////
// Simple gauge
///////////////////////////////////////////////////////////////////////////////

inline void draw_simple_gauge(
    display::Display &it,
    int center_x,
    int center_y,
    BaseFont *value_font,
    float percentage)
{
    GaugeData data;

    data.value = percentage;
    data.minimum = 0.0f;
    data.maximum = 100.0f;

    data.title = "";
    data.subtitle = "";
    data.unit = "%";
    data.icon = "";

    draw_gauge(
        it,
        center_x,
        center_y,
        value_font,
        value_font,
        value_font,
        battery_style(),
        data);
}

} // namespace ui