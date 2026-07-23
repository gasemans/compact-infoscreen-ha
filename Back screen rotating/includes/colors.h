#pragma once

#include "esphome.h"

namespace ui {

// Achtergrond
static const Color BACKGROUND(0, 0, 0);
static const Color BLACK(0, 0, 0);
static const Color WHITE(255, 255, 255);

// Tekst
static const Color TEXT(255, 255, 255);
static const Color TEXT_SECONDARY(170, 170, 170);
static const Color TEXT_DISABLED(90, 90, 90);

// Ringen / lijnen
static const Color RING_BACKGROUND(45, 45, 45);
static const Color DIVIDER(70, 70, 70);

// Statuskleuren
static const Color GREEN(45, 255, 100);
static const Color ORANGE(255, 180, 0);
static const Color RED(255, 60, 60);

// Extra accentkleuren
static const Color BLUE(24, 182, 255);
static const Color CYAN(0, 255, 255);
static const Color YELLOW(255, 216, 58);
static const Color PURPLE(180, 80, 255);

// Transparant (handig voor later)
static const Color NONE(0, 0, 0, 0);

// Kies automatisch een kleur op basis van een percentage
// < 5%  : rood
// 5-20% : oranje
// 20-50%: geel
// >=50% : groen
inline Color percentage_color(float value)
{
    if (value < 5.0f)
        return RED;

    if (value < 20.0f)
        return ORANGE;

    if (value < 50.0f)
        return YELLOW;

    return GREEN;
}

} // namespace ui