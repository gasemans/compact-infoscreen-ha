#pragma once

#include "esphome.h"

#include "colors.h"
#include "helpers.h"
#include "gauge_math.h"
#include "gauge_types.h"

namespace ui {

///////////////////////////////////////////////////////////////////////////////
// Configuration
///////////////////////////////////////////////////////////////////////////////

// 60 segmenten is ruim voldoende vloeiend op dit 240px scherm (straal ~84px)
// en scheelt flink in tekentijd t.o.v. de vorige 180 (zie "display took a
// long time" waarschuwing in de logs).
constexpr int ARC_SEGMENTS = 60;

///////////////////////////////////////////////////////////////////////////////
// Draw one radial line
///////////////////////////////////////////////////////////////////////////////

inline void draw_arc_segment(
    display::Display &it,
    int cx,
    int cy,
    int outer_radius,
    int inner_radius,
    float angle,
    Color color)
{
    Point outer = polar_to_cartesian(
        cx,
        cy,
        outer_radius,
        angle);

    Point inner = polar_to_cartesian(
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
// Draw complete arc
///////////////////////////////////////////////////////////////////////////////

inline void draw_arc(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style,
    float start_angle,
    float end_angle,
    Color color)
{
    if (end_angle < start_angle)
        std::swap(start_angle, end_angle);

    const float sweep = end_angle - start_angle;

    for (int i = 0; i <= ARC_SEGMENTS; i++)
    {
        float angle =
            start_angle +
            (sweep * i) /
            ARC_SEGMENTS;

        draw_arc_segment(
            it,
            cx,
            cy,
            outer_radius(style),
            inner_radius(style),
            angle,
            color);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw background
///////////////////////////////////////////////////////////////////////////////

inline void draw_background_arc(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style)
{
    draw_arc(
        it,
        cx,
        cy,
        style,
        style.start_angle,
        style.end_angle,
        style.background);
}

///////////////////////////////////////////////////////////////////////////////
// Draw foreground
///////////////////////////////////////////////////////////////////////////////

inline void draw_foreground_arc(
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

    float end_angle =
        style.start_angle +
        sweep_angle(style) *
        (percentage / 100.0f);

    draw_arc(
        it,
        cx,
        cy,
        style,
        style.start_angle,
        end_angle,
        percentage_color(percentage));
}

///////////////////////////////////////////////////////////////////////////////
// Draw partial arc
///////////////////////////////////////////////////////////////////////////////

inline void draw_arc_range(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style,
    float start_percentage,
    float end_percentage,
    Color color)
{
    start_percentage =
        clamp(start_percentage,0,100);

    end_percentage =
        clamp(end_percentage,0,100);

    float start_angle =
        style.start_angle +
        sweep_angle(style) *
        (start_percentage / 100.0f);

    float end_angle =
        style.start_angle +
        sweep_angle(style) *
        (end_percentage / 100.0f);

    draw_arc(
        it,
        cx,
        cy,
        style,
        start_angle,
        end_angle,
        color);
}

///////////////////////////////////////////////////////////////////////////////
// Rounded end caps
///////////////////////////////////////////////////////////////////////////////

inline void draw_arc_caps(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style,
    float percentage)
{
    if (!style.rounded)
        return;

    percentage =
        clamp(percentage,0,100);

    Point start =
        polar_to_cartesian(
            cx,
            cy,
            middle_radius(style),
            style.start_angle);

    float end_angle =
        style.start_angle +
        sweep_angle(style) *
        (percentage / 100.0f);

    Point end =
        polar_to_cartesian(
            cx,
            cy,
            middle_radius(style),
            end_angle);

    int radius =
        style.thickness / 2;

    it.filled_circle(
        start.x,
        start.y,
        radius,
        style.background);

    it.filled_circle(
        end.x,
        end.y,
        radius,
        percentage_color(percentage));
}

///////////////////////////////////////////////////////////////////////////////
// Draw gauge ring
///////////////////////////////////////////////////////////////////////////////

inline void draw_gauge_ring(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style,
    float percentage)
{
    draw_background_arc(
        it,
        cx,
        cy,
        style);

    draw_foreground_arc(
        it,
        cx,
        cy,
        style,
        percentage);

    draw_arc_caps(
        it,
        cx,
        cy,
        style,
        percentage);
}

} // namespace ui