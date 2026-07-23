#pragma once

#include "esphome.h"

#include "layouts.h"
#include "colors.h"

#include "battery_page.h"
#include "clock_page.h"
#include "weather_page.h"
#include "salt_page.h"

namespace ui {

inline void draw_page(
    display::Display &it,
    Page page,

    BaseFont *font_xl,
    BaseFont *font_lg,
    BaseFont *font_md,
    BaseFont *font_sm,
    BaseFont *font_sm_bold,

    float battery1,
    float battery2,

    float salt,

    float temperature,
    float humidity,
    float wind_speed,
    float wind_bearing,

    const char *weather_state,

    ESPTime now)
{
    it.fill(BACKGROUND);

    switch (page)
    {
        case Page::BATTERY:
            draw_battery_page(
                it,
                font_xl,
                font_lg,
                font_md,
                font_sm,
                font_sm_bold,
                battery1,
                battery2);
            break;

        case Page::CLOCK:
            draw_clock_page(
                it,
                font_xl,
                font_lg,
                font_md,
                font_sm,
                now);
            break;

        case Page::WEATHER:
            draw_weather_page(
                it,
                font_xl,
                font_lg,
                font_md,
                font_sm,
                temperature,
                humidity,
                wind_speed,
                wind_bearing,
                weather_state);
            break;

        case Page::SALT:
            draw_salt_page(
                it,
                font_xl,
                font_lg,
                font_md,
                font_sm,
                salt);
            break;
    }
}

} // namespace ui