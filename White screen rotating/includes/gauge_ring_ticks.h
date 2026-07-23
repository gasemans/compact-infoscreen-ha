#pragma once

#include "esphome.h"

#include "colors.h"
#include "helpers.h"
#include "gauge_lookup.h"
#include "gauge_math.h"
#include "gauge_types.h"

namespace ui {

///////////////////////////////////////////////////////////////////////////////
// Tick configuration
///////////////////////////////////////////////////////////////////////////////

constexpr int MAJOR_TICKS = 10;
constexpr int MINOR_TICKS = 50;

///////////////////////////////////////////////////////////////////////////////
// Draw one tick
///////////////////////////////////////////////////////////////////////////////

inline void draw_tick(
    display::Display &it,
    int cx,
    int cy,
    int angle,
    int outer_radius,
    int inner_radius,
    Color color)
{
    Point outer = lookup_point(
        cx,
        cy,
        outer_radius,
        angle);

    Point inner = lookup_point(
        cx,
        cy,
        inner_radius,
        angle);

    it.line(
        outer.x,
        outer.y,
        inner.x,
        inner.y,
        color);
}

///////////////////////////////////////////////////////////////////////////////
// Draw major ticks
///////////////////////////////////////////////////////////////////////////////

inline void draw_major_ticks(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style)
{
    const float sweep =
        sweep_angle(style);

    for (int i = 0; i <= MAJOR_TICKS; i++)
    {
        int angle =
            static_cast<int>(
                std::lround(
                    style.start_angle +
                    (sweep * i) /
                    MAJOR_TICKS));

        draw_tick(
            it,
            cx,
            cy,
            angle,
            style.radius + 2,
            style.radius - 10,
            TEXT);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw minor ticks
///////////////////////////////////////////////////////////////////////////////

inline void draw_minor_ticks(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style)
{
    const float sweep =
        sweep_angle(style);

    for (int i = 0; i <= MINOR_TICKS; i++)
    {
        if ((i % 5) == 0)
            continue;

        int angle =
            static_cast<int>(
                std::lround(
                    style.start_angle +
                    (sweep * i) /
                    MINOR_TICKS));

        draw_tick(
            it,
            cx,
            cy,
            angle,
            style.radius + 1,
            style.radius - 5,
            TEXT_DISABLED);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw percentage progress ticks
///////////////////////////////////////////////////////////////////////////////

inline void draw_progress_ticks(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style,
    float percentage)
{
    percentage = clamp(
        percentage,
        0.0f,
        100.0f);

    const int active_ticks =
        clamp_int(
            static_cast<int>(
                std::lround(
                    (percentage / 100.0f) *
                    MINOR_TICKS)),
            0,
            MINOR_TICKS);

    const float sweep =
        sweep_angle(style);

    Color color =
        percentage_color(percentage);

    for (int i = 0; i <= active_ticks; i++)
    {
        int angle =
            static_cast<int>(
                std::lround(
                    style.start_angle +
                    (sweep * i) /
                    MINOR_TICKS));

        draw_tick(
            it,
            cx,
            cy,
            angle,
            style.radius,
            style.radius - style.thickness,
            color);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw all ticks
///////////////////////////////////////////////////////////////////////////////

inline void draw_ticks(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style)
{
    if (!style.show_ticks)
        return;

    draw_minor_ticks(
        it,
        cx,
        cy,
        style);

    draw_major_ticks(
        it,
        cx,
        cy,
        style);
}

///////////////////////////////////////////////////////////////////////////////
// Draw decorative scale
///////////////////////////////////////////////////////////////////////////////

inline void draw_scale(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style,
    float percentage)
{
    draw_progress_ticks(
        it,
        cx,
        cy,
        style,
        percentage);

    draw_major_ticks(
        it,
        cx,
        cy,
        style);
}

} // namespace ui