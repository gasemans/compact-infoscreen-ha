#pragma once

#include "esphome.h"

#include "colors.h"
#include "helpers.h"

#include "gauge_types.h"
#include "gauge_math.h"

#include "gauge_lookup.h"
#include "gauge_ring_arc.h"
#include "gauge_ring_ticks.h"
#include "gauge_ring_shadow.h"

namespace ui {

///////////////////////////////////////////////////////////////////////////////
// Draw complete ring
///////////////////////////////////////////////////////////////////////////////

inline void draw_ring(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style,
    float percentage)
{
    percentage = clamp(percentage, 0.0f, 100.0f);

    if (style.show_shadow)
    {
        draw_soft_shadow(
            it,
            cx,
            cy,
            style);
    }

    draw_gauge_ring(
        it,
        cx,
        cy,
        style,
        percentage);

    if (style.show_ticks)
    {
        draw_scale(
            it,
            cx,
            cy,
            style,
            percentage);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Compatibility wrappers
///////////////////////////////////////////////////////////////////////////////

inline void draw_ring_background(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style)
{
    draw_background_arc(
        it,
        cx,
        cy,
        style);
}

inline void draw_ring_value(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style,
    float percentage)
{
    draw_foreground_arc(
        it,
        cx,
        cy,
        style,
        percentage);
}

inline void draw_end_caps(
    display::Display &it,
    int cx,
    int cy,
    const GaugeStyle &style,
    float percentage)
{
    draw_arc_caps(
        it,
        cx,
        cy,
        style,
        percentage);
}

} // namespace ui