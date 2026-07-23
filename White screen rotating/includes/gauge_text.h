#pragma once

#include "esphome.h"

#include "colors.h"
#include "layouts.h"
#include "helpers.h"
#include "gauge_types.h"

namespace ui {

///////////////////////////////////////////////////////////////////////////////
// Title
///////////////////////////////////////////////////////////////////////////////

inline void draw_title(
    display::Display &it,
    int x,
    int y,
    BaseFont *font,
    Color color,
    const char *title)
{
    if (title == nullptr || title[0] == '\0')
        return;

    it.printf(
        x,
        y,
        font,
        color,
        TextAlign::CENTER,
        "%s",
        title);
}

///////////////////////////////////////////////////////////////////////////////
// Subtitle
///////////////////////////////////////////////////////////////////////////////

inline void draw_subtitle(
    display::Display &it,
    int x,
    int y,
    BaseFont *font,
    Color color,
    const char *subtitle)
{
    if (subtitle == nullptr || subtitle[0] == '\0')
        return;

    it.printf(
        x,
        y,
        font,
        color,
        TextAlign::CENTER,
        "%s",
        subtitle);
}

///////////////////////////////////////////////////////////////////////////////
// Icon
///////////////////////////////////////////////////////////////////////////////

inline void draw_icon(
    display::Display &it,
    int x,
    int y,
    BaseFont *font,
    Color color,
    const char *icon)
{
    if (icon == nullptr || icon[0] == '\0')
        return;

    it.printf(
        x,
        y,
        font,
        color,
        TextAlign::CENTER,
        "%s",
        icon);
}

///////////////////////////////////////////////////////////////////////////////
// Value
///////////////////////////////////////////////////////////////////////////////

inline void draw_value(
    display::Display &it,
    int x,
    int y,
    BaseFont *font,
    Color color,
    float value)
{
    if (!is_valid(value))
    {
        it.print(
            x,
            y,
            font,
            RED,
            TextAlign::CENTER,
            "--");

        return;
    }

    it.printf(
        x,
        y,
        font,
        color,
        TextAlign::CENTER,
        "%.0f",
        value);
}

///////////////////////////////////////////////////////////////////////////////
// Value + Unit
///////////////////////////////////////////////////////////////////////////////

inline void draw_value_unit(
    display::Display &it,
    int x,
    int y,
    BaseFont *value_font,
    BaseFont *unit_font,
    Color color,
    float value,
    const char *unit)
{
    if (!is_valid(value))
    {
        it.print(
            x,
            y,
            value_font,
            RED,
            TextAlign::CENTER,
            "--");

        return;
    }

    it.printf(
        x,
        y,
        value_font,
        color,
        TextAlign::CENTER,
        "%.0f%s",
        value,
        unit);
}

///////////////////////////////////////////////////////////////////////////////
// Percentage
///////////////////////////////////////////////////////////////////////////////

inline void draw_percentage(
    display::Display &it,
    int x,
    int y,
    BaseFont *font,
    float value)
{
    draw_value_unit(
        it,
        x,
        y,
        font,
        font,
        percentage_color(value),
        value,
        "%");
}

///////////////////////////////////////////////////////////////////////////////
// Temperature
///////////////////////////////////////////////////////////////////////////////

inline void draw_temperature(
    display::Display &it,
    int x,
    int y,
    BaseFont *font,
    float value)
{
    if (!is_valid(value))
    {
        it.print(
            x,
            y,
            font,
            RED,
            TextAlign::CENTER,
            "--");

        return;
    }

    it.printf(
        x,
        y,
        font,
        temperature_color(value),
        TextAlign::CENTER,
        "%.1f°C",
        value);
}

///////////////////////////////////////////////////////////////////////////////
// Center text
///////////////////////////////////////////////////////////////////////////////

inline void draw_center_text(
    display::Display &it,
    BaseFont *title_font,
    BaseFont *value_font,
    BaseFont *subtitle_font,
    int center_x,
    int center_y,
    const GaugeData &data,
    Color value_color)
{
    if (data.title[0] != '\0')
    {
        draw_title(
            it,
            center_x,
            center_y - 46,
            title_font,
            TEXT_SECONDARY,
            data.title);
    }

    if (is_valid(data.value))
    {
        it.printf(
            center_x,
            center_y,
            value_font,
            value_color,
            TextAlign::CENTER,
            "%.0f%s",
            data.value,
            data.unit);
    }
    else
    {
        it.print(
            center_x,
            center_y,
            value_font,
            RED,
            TextAlign::CENTER,
            "--");
    }

    if (data.subtitle[0] != '\0')
    {
        draw_subtitle(
            it,
            center_x,
            center_y + 40,
            subtitle_font,
            TEXT_SECONDARY,
            data.subtitle);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Bottom label/value
///////////////////////////////////////////////////////////////////////////////

inline void draw_bottom_value(
    display::Display &it,
    int x,
    int label_y,
    int value_y,
    BaseFont *label_font,
    BaseFont *value_font,
    const char *label,
    float value)
{
    it.print(
        x,
        label_y,
        label_font,
        TEXT_SECONDARY,
        TextAlign::CENTER,
        label);

    if (is_valid(value))
    {
        it.printf(
            x,
            value_y,
            value_font,
            percentage_color(value),
            TextAlign::CENTER,
            "%.0f%%",
            value);
    }
    else
    {
        it.print(
            x,
            value_y,
            value_font,
            RED,
            TextAlign::CENTER,
            "--");
    }
}

} // namespace ui