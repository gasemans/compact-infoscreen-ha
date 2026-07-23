#pragma once

#include "esphome.h"

#include "colors.h"
#include "layouts.h"
#include "helpers.h"
#include "icons.h"
#include "gauge.h"

namespace ui {

///////////////////////////////////////////////////////////////////////////////
// Eén doorlopende ring (grijze track + gekleurde vulling o.b.v. het totale
// percentage), optisch verdeeld in een linker- en rechterboog door een
// kleine opening boven en onder. Batterij-icoon bovenin, totaal percentage
// groot in wit, "Accu" als subtitel, en de losse percentages van accu 1 en
// 2 onderin (zonder "B1"/"B2" label), elk gekleurd volgens hun eigen
// percentage.
///////////////////////////////////////////////////////////////////////////////

namespace battery_layout {

constexpr int RING_RADIUS    = 84;
constexpr int RING_THICKNESS = 14;

constexpr float GAP = 18.0f;   // opening boven en onder, samen

constexpr float RIGHT_BOTTOM =  (180.0f - GAP / 2.0f);   // 171°
constexpr float RIGHT_TOP    =  (GAP / 2.0f);             //   9°
constexpr float LEFT_TOP     = -(GAP / 2.0f);              //  -9°
constexpr float LEFT_BOTTOM  = -(180.0f - GAP / 2.0f);      // -171°

constexpr float HALF_SWEEP = RIGHT_BOTTOM - RIGHT_TOP;    // 162°

constexpr int ICON_Y  = CENTER_Y - 46;
constexpr int TOTAL_Y = CENTER_Y - 4;
constexpr int ACCU_Y  = CENTER_Y + 34;

// Ring loopt tot ongeveer y=203; percentages komen er duidelijk onder.
constexpr int SPLIT_Y = CENTER_Y + 92;

constexpr int SPLIT_LEFT_X  = CENTER_X - 24;
constexpr int SPLIT_RIGHT_X = CENTER_X + 24;

} // namespace battery_layout

///////////////////////////////////////////////////////////////////////////////
// Ring: grijze track + vulling die start rechtsonder, over rechts/boven/
// links loopt en eindigt linksonder bij 100%.
///////////////////////////////////////////////////////////////////////////////

inline void draw_battery_ring(
    display::Display &it,
    float total_percentage)
{
    using namespace battery_layout;

    GaugeStyle style = generic_style();
    style.radius   = RING_RADIUS;
    style.thickness = RING_THICKNESS;

    // Grijze achtergrond-track: beide helften
    draw_arc(it, CENTER_X, CENTER_Y, style, RIGHT_TOP, RIGHT_BOTTOM, RING_BACKGROUND);
    draw_arc(it, CENTER_X, CENTER_Y, style, LEFT_BOTTOM, LEFT_TOP,   RING_BACKGROUND);

    const float pct = clamp(total_percentage, 0.0f, 100.0f);
    const Color fill_color = percentage_color(pct);
    const float t = pct / 100.0f;

    if (t <= 0.0f)
        return;

    if (t <= 0.5f)
    {
        const float frac = t / 0.5f;
        const float edge = RIGHT_BOTTOM - frac * HALF_SWEEP;

        draw_arc(it, CENTER_X, CENTER_Y, style, edge, RIGHT_BOTTOM, fill_color);
    }
    else
    {
        draw_arc(it, CENTER_X, CENTER_Y, style, RIGHT_TOP, RIGHT_BOTTOM, fill_color);

        const float frac = (t - 0.5f) / 0.5f;
        const float edge = LEFT_TOP - frac * HALF_SWEEP;

        draw_arc(it, CENTER_X, CENTER_Y, style, edge, LEFT_TOP, fill_color);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Batterijpagina
///////////////////////////////////////////////////////////////////////////////

inline void draw_battery_page(
    display::Display &it,
    BaseFont *value_font,
    BaseFont *title_font,
    BaseFont *subtitle_font,
    BaseFont *small_font,
    BaseFont *bold_font,
    float battery1,
    float battery2)
{
    using namespace battery_layout;

    it.fill(WHITE);

    const float b1 = sanitize(battery1);
    const float b2 = sanitize(battery2);
    const float total = average(b1, b2);

    draw_battery_ring(it, total);

    // Batterij-icoon bovenin, gekleurd op basis van het totaal
    draw_icon(
        it,
        CENTER_X,
        ICON_Y,
        subtitle_font,
        percentage_color(total),
        icon::BATTERY);

    // Totaal percentage, groot en wit
    if (is_valid(total))
    {
        it.printf(
            CENTER_X,
            TOTAL_Y,
            value_font,
            BLACK,
            TextAlign::CENTER,
            "%.0f%%",
            total);
    }
    else
    {
        it.print(
            CENTER_X,
            TOTAL_Y,
            value_font,
            BLACK,
            TextAlign::CENTER,
            "--");
    }

    // Subtitel
    it.print(
        CENTER_X,
        ACCU_Y,
        small_font,
        TEXT_SECONDARY,
        TextAlign::CENTER,
        "Accu");

    // Verdeler
    it.line(
        CENTER_X, SPLIT_Y - 10,
        CENTER_X, SPLIT_Y + 10,
        DIVIDER);

    // Losse percentages, elk gekleurd op basis van hun eigen waarde,
    // iets dikker gedrukt, onder de ring.
    if (is_valid(b1))
    {
        it.printf(
            SPLIT_LEFT_X,
            SPLIT_Y,
            bold_font,
            percentage_color(b1),
            TextAlign::CENTER,
            "%.0f%%",
            b1);
    }

    if (is_valid(b2))
    {
        it.printf(
            SPLIT_RIGHT_X,
            SPLIT_Y,
            bold_font,
            percentage_color(b2),
            TextAlign::CENTER,
            "%.0f%%",
            b2);
    }
}

} // namespace ui
