#pragma once

#include "esphome.h"

#include "colors.h"
#include "layouts.h"
#include "helpers.h"
#include "gauge_lookup.h"

namespace ui {

///////////////////////////////////////////////////////////////////////////////
// Stationsklok (analoog, geen tekst/datum)
//
// Lichte variant (witte achtergrond, zwarte tekst):
// - witte wijzerplaat
// - dikke zwarte uurstrepen (12x), dunnere minuutstrepen (48x)
// - zwarte, getaperde uur- en minutenwijzer
// - dunne rode secondewijzer met rode "stopdisc" op de as
///////////////////////////////////////////////////////////////////////////////

namespace clock_geom {

constexpr int   FACE_RADIUS       = OUTER_RADIUS;   // 92

constexpr float HOUR_TICK_OUTER   = FACE_RADIUS - 2.0f;
constexpr float HOUR_TICK_INNER   = FACE_RADIUS - 24.0f;
constexpr float HOUR_TICK_WIDTH   = 7.0f;

constexpr float TWELVE_TICK_INNER = FACE_RADIUS - 36.0f;  // iets langer dan de andere uurstrepen

constexpr float MINUTE_TICK_OUTER = FACE_RADIUS - 2.0f;
constexpr float MINUTE_TICK_INNER = FACE_RADIUS - 12.0f;
constexpr float MINUTE_TICK_WIDTH = 2.5f;

constexpr float HOUR_HAND_LENGTH   = 48.0f;
constexpr float HOUR_HAND_TAIL     = -8.0f;
constexpr float HOUR_HAND_WIDTH_IN = 8.0f;
constexpr float HOUR_HAND_WIDTH_OUT= 5.0f;

constexpr float MINUTE_HAND_LENGTH  = 80.0f;
constexpr float MINUTE_HAND_TAIL    = -10.0f;
constexpr float MINUTE_HAND_WIDTH_IN = 7.0f;
constexpr float MINUTE_HAND_WIDTH_OUT= 3.0f;

constexpr float SECOND_HAND_LENGTH = 66.0f;
constexpr float SECOND_HAND_TAIL   = -20.0f;

constexpr int HUB_RADIUS = 7;

} // namespace clock_geom

///////////////////////////////////////////////////////////////////////////////
// Hoek-helper: 0° = 12 uur (boven), oplopend met de klok mee
///////////////////////////////////////////////////////////////////////////////

inline LookupPoint clock_direction(float clock_angle_deg)
{
    const int math_angle =
        static_cast<int>(
            std::lround(90.0f - clock_angle_deg));

    return lookup(math_angle);
}

inline void clock_endpoint(
    int cx,
    int cy,
    float clock_angle_deg,
    float radius,
    int &x,
    int &y)
{
    const auto dir = clock_direction(clock_angle_deg);

    x = cx + static_cast<int>(std::lround(dir.c * radius));
    y = cy - static_cast<int>(std::lround(dir.s * radius));
}

///////////////////////////////////////////////////////////////////////////////
// Getaperde balk tekenen (voor wijzers en dikke uurstrepen)
// inner_radius mag negatief zijn: dat geeft een "staartje" aan de
// andere kant van het middelpunt (zoals bij echte wijzers).
///////////////////////////////////////////////////////////////////////////////

inline void draw_tapered_bar(
    display::Display &it,
    int cx,
    int cy,
    float clock_angle_deg,
    float inner_radius,
    float outer_radius,
    float width_inner,
    float width_outer,
    Color color)
{
    const auto dir = clock_direction(clock_angle_deg);

    // Loodrechte eenheidsvector op het scherm (voor de breedte van de balk)
    const float px = dir.s;
    const float py = dir.c;

    const float ix = cx + dir.c * inner_radius;
    const float iy = cy - dir.s * inner_radius;
    const float ox = cx + dir.c * outer_radius;
    const float oy = cy - dir.s * outer_radius;

    const int ix1 = static_cast<int>(std::lround(ix + px * (width_inner * 0.5f)));
    const int iy1 = static_cast<int>(std::lround(iy + py * (width_inner * 0.5f)));
    const int ix2 = static_cast<int>(std::lround(ix - px * (width_inner * 0.5f)));
    const int iy2 = static_cast<int>(std::lround(iy - py * (width_inner * 0.5f)));

    const int ox1 = static_cast<int>(std::lround(ox + px * (width_outer * 0.5f)));
    const int oy1 = static_cast<int>(std::lround(oy + py * (width_outer * 0.5f)));
    const int ox2 = static_cast<int>(std::lround(ox - px * (width_outer * 0.5f)));
    const int oy2 = static_cast<int>(std::lround(oy - py * (width_outer * 0.5f)));

    it.filled_triangle(ix1, iy1, ix2, iy2, ox1, oy1, color);
    it.filled_triangle(ix2, iy2, ox1, oy1, ox2, oy2, color);
}

///////////////////////////////////////////////////////////////////////////////
// Wijzerplaat: 60 streepjes, 12 dik (uur), 48 dun (minuut)
///////////////////////////////////////////////////////////////////////////////

inline void draw_clock_ticks(
    display::Display &it,
    int cx,
    int cy)
{
    using namespace clock_geom;

    for (int i = 0; i < 60; i++)
    {
        const float angle = i * 6.0f;

        if ((i % 5) == 0)
        {
            const float inner =
                (i == 0)
                    ? TWELVE_TICK_INNER
                    : HOUR_TICK_INNER;

            draw_tapered_bar(
                it,
                cx,
                cy,
                angle,
                inner,
                HOUR_TICK_OUTER,
                HOUR_TICK_WIDTH,
                HOUR_TICK_WIDTH,
                BLACK);
        }
        else
        {
            draw_tapered_bar(
                it,
                cx,
                cy,
                angle,
                MINUTE_TICK_INNER,
                MINUTE_TICK_OUTER,
                MINUTE_TICK_WIDTH,
                MINUTE_TICK_WIDTH,
                BLACK);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Wijzers
///////////////////////////////////////////////////////////////////////////////

inline void draw_clock_hands(
    display::Display &it,
    int cx,
    int cy,
    float hour_angle,
    float minute_angle,
    float second_angle)
{
    using namespace clock_geom;

    // Uurwijzer
    draw_tapered_bar(
        it,
        cx,
        cy,
        hour_angle,
        HOUR_HAND_TAIL,
        HOUR_HAND_LENGTH,
        HOUR_HAND_WIDTH_IN,
        HOUR_HAND_WIDTH_OUT,
        BLACK);

    // Minutenwijzer
    draw_tapered_bar(
        it,
        cx,
        cy,
        minute_angle,
        MINUTE_HAND_TAIL,
        MINUTE_HAND_LENGTH,
        MINUTE_HAND_WIDTH_IN,
        MINUTE_HAND_WIDTH_OUT,
        BLACK);

    // Secondewijzer (dun, rood)
    int tail_x, tail_y, tip_x, tip_y;

    clock_endpoint(cx, cy, second_angle, SECOND_HAND_TAIL, tail_x, tail_y);
    clock_endpoint(cx, cy, second_angle, SECOND_HAND_LENGTH, tip_x, tip_y);

    it.line(tail_x, tail_y, tip_x, tip_y, RED);

    // Rode "stopdisc" op de as, bovenop de wijzers
    it.filled_circle(cx, cy, HUB_RADIUS, RED);
}

///////////////////////////////////////////////////////////////////////////////
// Pagina
///////////////////////////////////////////////////////////////////////////////

inline void draw_clock_page(
    display::Display &it,
    BaseFont *font_xl,
    BaseFont *font_lg,
    BaseFont *font_md,
    BaseFont *font_sm,
    ESPTime now)
{
    // Op deze pagina gebruiken we bewust geen tekst/iconen,
    // dus deze fonts blijven ongebruikt.
    (void) font_xl;
    (void) font_lg;
    (void) font_md;
    (void) font_sm;

    ///////////////////////////////////////////////////////////////////////////
    // Witte wijzerplaat
    ///////////////////////////////////////////////////////////////////////////

    it.fill(WHITE);

    ///////////////////////////////////////////////////////////////////////////
    // Hoeken berekenen (0° = 12 uur, met de klok mee)
    ///////////////////////////////////////////////////////////////////////////

    const float seconds_fraction = now.second;

    const float total_minutes =
        static_cast<float>(now.hour % 12) * 60.0f +
        static_cast<float>(now.minute) +
        seconds_fraction / 60.0f;

    const float hour_angle   = total_minutes * 0.5f;              // 360° / (12*60min)
    const float minute_angle = (now.minute + seconds_fraction / 60.0f) * 6.0f; // 360° / 60min
    const float second_angle = seconds_fraction * 6.0f;           // 360° / 60s

    ///////////////////////////////////////////////////////////////////////////
    // Tekenen
    ///////////////////////////////////////////////////////////////////////////

    draw_clock_ticks(it, CENTER_X, CENTER_Y);

    draw_clock_hands(
        it,
        CENTER_X,
        CENTER_Y,
        hour_angle,
        minute_angle,
        second_angle);
}

} // namespace ui
