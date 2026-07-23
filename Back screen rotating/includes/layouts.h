#pragma once

#include "esphome.h"

namespace ui {

// Display afmetingen
constexpr int SCREEN_WIDTH  = 240;
constexpr int SCREEN_HEIGHT = 240;

// Midden van het ronde display
constexpr int CENTER_X = SCREEN_WIDTH / 2;
constexpr int CENTER_Y = SCREEN_HEIGHT / 2;

// Buitenste ring
constexpr int OUTER_RADIUS = 92;

// Gauge
constexpr int GAUGE_RADIUS = 84;
constexpr int GAUGE_THICKNESS = 14;

// Tekstposities batterijpagina
constexpr int TITLE_Y = 22;
constexpr int VALUE_Y = 78;
constexpr int SUBTITLE_Y = 138;

constexpr int BATTERY1_X = 50;
constexpr int BATTERY2_X = 190;

constexpr int BATTERY_LABEL_Y = 184;
constexpr int BATTERY_VALUE_Y = 206;

// Algemene marges
constexpr int TOP_MARGIN = 18;
constexpr int BOTTOM_MARGIN = 18;

// Pagina's
enum class Page : uint8_t
{
    BATTERY = 0,
    CLOCK   = 1,
    WEATHER = 2,
    SALT    = 3
};

} // namespace ui