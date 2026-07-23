#pragma once

#include "esphome.h"

#include "colors.h"
#include "layouts.h"
#include "helpers.h"
#include "icons.h"
#include "gauge.h"

namespace ui {

///////////////////////////////////////////////////////////////////////////////
// Zoutwaterniveau: zwarte achtergrond, witte tekst. 260° gauge van links
// naar rechts (over de bovenkant). Waterdruppel als icoon. Percentage
// altijd zichtbaar (wit); "REFILL" verschijnt alleen onder de 5%.
///////////////////////////////////////////////////////////////////////////////

namespace salt_layout {

constexpr int RING_RADIUS    = 84;
constexpr int RING_THICKNESS = 16;

// Hoeken in "klok-conventie" (0° = 12 uur, met de klok mee).
// 230° -> 490° = 260° sweep: start net onder "links" (9 uur),
// via boven, naar net onder "rechts" (3 uur). Gat van 100° onderin.
constexpr float GAUGE_START_ANGLE = 230.0f;
constexpr float GAUGE_END_ANGLE   = 490.0f;

constexpr float LOW_THRESHOLD = 5.0f;

constexpr int ICON_Y    = CENTER_Y - 30;
constexpr int PCT_Y     = CENTER_Y + 14;
constexpr int REFILL_Y  = CENTER_Y + 50;

} // namespace salt_layout

///////////////////////////////////////////////////////////////////////////////
// Salt page
///////////////////////////////////////////////////////////////////////////////

inline void draw_salt_page(
    display::Display &it,
    BaseFont *value_font,
    BaseFont *title_font,
    BaseFont *subtitle_font,
    BaseFont *small_font,
    float salt_percentage)
{
    using namespace salt_layout;

    const float percentage = sanitize(salt_percentage);

    it.fill(BLACK);

    GaugeStyle style = generic_style();

    style.radius      = RING_RADIUS;
    style.thickness    = RING_THICKNESS;
    style.start_angle    = GAUGE_START_ANGLE;
    style.end_angle        = GAUGE_END_ANGLE;
    style.background        = BLACK;
    style.rounded             = true;
    style.show_shadow          = false;
    style.show_ticks            = false;

    draw_ring(
        it,
        CENTER_X,
        CENTER_Y,
        style,
        percentage);

    draw_icon(
        it,
        CENTER_X,
        ICON_Y,
        title_font,
        WHITE,
        icon::WATER);

    if (is_valid(percentage))
    {
        it.printf(
            CENTER_X,
            PCT_Y,
            value_font,
            WHITE,
            TextAlign::CENTER,
            "%.0f%%",
            percentage);
    }
    else
    {
        it.print(
            CENTER_X,
            PCT_Y,
            value_font,
            WHITE,
            TextAlign::CENTER,
            "--");
    }

    if (is_valid(percentage) && percentage < LOW_THRESHOLD)
    {
        it.print(
            CENTER_X,
            REFILL_Y,
            subtitle_font,
            WHITE,
            TextAlign::CENTER,
            "REFILL");
    }
}

} // namespace ui
