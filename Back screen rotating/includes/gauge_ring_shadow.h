#pragma once

#include "esphome.h"

#include "colors.h"
#include "helpers.h"
#include "gauge_lookup.h"
#include "gauge_math.h"
#include "gauge_types.h"

namespace ui {

///////////////////////////////////////////////////////////////////////////////
// Shadow configuration
///////////////////////////////////////////////////////////////////////////////

constexpr int SHADOW_OFFSET_X = 2;
constexpr int SHADOW_OFFSET_Y = 2;

///////////////////////////////////////////////////////////////////////////////
// Draw one shadow segment
///////////////////////////////////////////////////////////////////////////////

inline void draw_shadow_segment(
    display::Display &it,
    int cx,
    int cy,
    int outer_radius,
    int inner_radius,
    int angle,
    Color color)
{
    Point outer = lookup_point(
        cx + SHADOW_OFFSET_X,
        cy + SHADOW_OFFSET_Y,
        outer_radius,
        angle);

    Point inner = lookup_point(
        cx + SHADOW_OFFSET_X,
        cy + SHADOW_OFFSET_Y,
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
// Draw complete shadow arc
///////////////////////////////////////////////////////////////////////////////

inline void draw_shadow(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style)
{
    if (!style.show_shadow)
        return;

    constexpr Color SHADOW_COLOR(18, 18, 18);

    const float sweep = sweep_angle(style);

    constexpr int SEGMENTS = 180;

    for (int i = 0; i <= SEGMENTS; i++)
    {
        int angle =
            static_cast<int>(
                std::lround(
                    style.start_angle +
                    (sweep * i) / SEGMENTS));

        draw_shadow_segment(
            it,
            cx,
            cy,
            outer_radius(style),
            inner_radius(style),
            angle,
            SHADOW_COLOR);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Soft shadow (multiple passes)
///////////////////////////////////////////////////////////////////////////////

inline void draw_soft_shadow(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style)
{
    if (!style.show_shadow)
        return;

    constexpr Color SHADOW1(24,24,24);
    constexpr Color SHADOW2(16,16,16);
    constexpr Color SHADOW3(8,8,8);

    GaugeStyle shadow_style = style;

    // Pass 1
    draw_shadow(it, cx + 1, cy + 1, shadow_style);

    // Pass 2
    shadow_style.radius += 1;

    const float sweep = sweep_angle(shadow_style);

    constexpr int SEGMENTS = 180;

    for (int i = 0; i <= SEGMENTS; i++)
    {
        int angle =
            static_cast<int>(
                std::lround(
                    shadow_style.start_angle +
                    (sweep * i) / SEGMENTS));

        draw_shadow_segment(
            it,
            cx + 2,
            cy + 2,
            outer_radius(shadow_style),
            inner_radius(shadow_style),
            angle,
            SHADOW2);
    }

    // Pass 3
    shadow_style.radius += 1;

    for (int i = 0; i <= SEGMENTS; i++)
    {
        int angle =
            static_cast<int>(
                std::lround(
                    shadow_style.start_angle +
                    (sweep * i) / SEGMENTS));

        draw_shadow_segment(
            it,
            cx + 3,
            cy + 3,
            outer_radius(shadow_style),
            inner_radius(shadow_style),
            angle,
            SHADOW3);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw glow
///////////////////////////////////////////////////////////////////////////////

inline void draw_glow(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style,
    Color color)
{
    GaugeStyle glow = style;

    glow.radius += 2;

    const float sweep = sweep_angle(glow);

    constexpr int SEGMENTS = 180;

    for (int i = 0; i <= SEGMENTS; i++)
    {
        int angle =
            static_cast<int>(
                std::lround(
                    glow.start_angle +
                    (sweep * i) / SEGMENTS));

        draw_shadow_segment(
            it,
            cx,
            cy,
            outer_radius(glow),
            inner_radius(glow),
            angle,
            color);
    }
}

} // namespace ui