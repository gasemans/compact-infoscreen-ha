#pragma once

#include "esphome.h"

#include "colors.h"
#include "layouts.h"
#include "icons.h"
#include "helpers.h"
#include "gauge_text.h"

namespace ui {

///////////////////////////////////////////////////////////////////////////////
// Weather icon + kleur + Nederlandse omschrijving
///////////////////////////////////////////////////////////////////////////////

inline const char *weather_icon(const char *condition)
{
    if (condition == nullptr)
        return icon::WEATHER;

    if (!strcmp(condition, "sunny"))
        return icon::SUN;

    if (!strcmp(condition, "clear-night"))
        return icon::MOON;

    if (!strcmp(condition, "cloudy"))
        return icon::CLOUD;

    if (!strcmp(condition, "partlycloudy"))
        return icon::PARTLY_CLOUDY;

    if (!strcmp(condition, "rainy"))
        return icon::RAIN;

    if (!strcmp(condition, "pouring"))
        return icon::RAIN;

    if (!strcmp(condition, "lightning"))
        return icon::LIGHTNING;

    if (!strcmp(condition, "lightning-rainy"))
        return icon::LIGHTNING;

    if (!strcmp(condition, "snowy"))
        return icon::SNOW;

    if (!strcmp(condition, "fog"))
        return icon::FOG;

    if (!strcmp(condition, "windy"))
        return icon::WIND;

    return icon::WEATHER;
}

inline Color weather_icon_color(const char *condition)
{
    if (condition == nullptr)
        return BLACK;

    if (!strcmp(condition, "sunny"))
        return YELLOW;

    if (!strcmp(condition, "clear-night"))
        return CYAN;

    if (!strcmp(condition, "partlycloudy"))
        return YELLOW;

    if (!strcmp(condition, "cloudy"))
        return TEXT_SECONDARY;

    if (!strcmp(condition, "rainy") || !strcmp(condition, "pouring"))
        return BLUE;

    if (!strcmp(condition, "lightning"))
        return YELLOW;

    if (!strcmp(condition, "lightning-rainy"))
        return ORANGE;

    if (!strcmp(condition, "snowy"))
        return CYAN;

    if (!strcmp(condition, "fog"))
        return TEXT_SECONDARY;

    if (!strcmp(condition, "windy"))
        return CYAN;

    return BLACK;
}

inline const char *weather_condition_text(const char *condition)
{
    if (condition == nullptr)
        return "Onbekend";

    if (!strcmp(condition, "sunny"))
        return "Zonnig";

    if (!strcmp(condition, "clear-night"))
        return "Helder";

    if (!strcmp(condition, "cloudy"))
        return "Bewolkt";

    if (!strcmp(condition, "partlycloudy"))
        return "Licht bewolkt";

    if (!strcmp(condition, "rainy"))
        return "Regenachtig";

    if (!strcmp(condition, "pouring"))
        return "Zware regen";

    if (!strcmp(condition, "lightning"))
        return "Onweer";

    if (!strcmp(condition, "lightning-rainy"))
        return "Onweer met regen";

    if (!strcmp(condition, "snowy"))
        return "Sneeuw";

    if (!strcmp(condition, "snowy-rainy"))
        return "Natte sneeuw";

    if (!strcmp(condition, "fog"))
        return "Mist";

    if (!strcmp(condition, "windy") || !strcmp(condition, "windy-variant"))
        return "Winderig";

    if (!strcmp(condition, "hail"))
        return "Hagel";

    if (!strcmp(condition, "exceptional"))
        return "Uitzonderlijk";

    // Onbekende/nieuwe HA-conditie: toon de ruwe waarde als terugval
    return condition;
}

///////////////////////////////////////////////////////////////////////////////
// Windrichting: graden -> Nederlandse windrichting (8-punts)
///////////////////////////////////////////////////////////////////////////////

inline const char *wind_direction_text(float bearing_deg)
{
    if (!is_valid(bearing_deg))
        return "--";

    float b = std::fmod(bearing_deg, 360.0f);

    if (b < 0.0f)
        b += 360.0f;

    static const char *dirs[8] = {"N", "NO", "O", "ZO", "Z", "ZW", "W", "NW"};

    int index = static_cast<int>(std::lround(b / 45.0f)) % 8;

    return dirs[index];
}

///////////////////////////////////////////////////////////////////////////////
// Layout: puur het huidige weer. Geen titel, geen verwachting, geen
// paginapuntjes. Zwarte achtergrond, witte tekst.
///////////////////////////////////////////////////////////////////////////////

namespace weather_layout {

constexpr int ICON_Y      = CENTER_Y - 78;
constexpr int TEMP_Y      = CENTER_Y - 26;
constexpr int CONDITION_Y = CENTER_Y + 14;

constexpr int DETAIL_ICON_Y  = CENTER_Y + 48;
constexpr int DETAIL_VALUE_Y = CENTER_Y + 68;

constexpr int LEFT_X  = CENTER_X - 48;
constexpr int RIGHT_X = CENTER_X + 48;

} // namespace weather_layout

///////////////////////////////////////////////////////////////////////////////
// Weather page
///////////////////////////////////////////////////////////////////////////////

inline void draw_weather_page(
    display::Display &it,
    BaseFont *title_font,
    BaseFont *value_font,
    BaseFont *subtitle_font,
    BaseFont *small_font,
    float temperature,
    float humidity,
    float wind_speed,
    float wind_bearing,
    const char *condition)
{
    using namespace weather_layout;

    it.fill(WHITE);

    // Weericoon, gekleurd naar het type weer
    draw_icon(
        it,
        CENTER_X,
        ICON_Y,
        value_font,
        weather_icon_color(condition),
        weather_icon(condition));

    // Huidige temperatuur
    if (is_valid(temperature))
    {
        it.printf(
            CENTER_X,
            TEMP_Y,
            value_font,
            BLACK,
            TextAlign::CENTER,
            "%.1f°C",
            temperature);
    }
    else
    {
        it.print(
            CENTER_X,
            TEMP_Y,
            value_font,
            BLACK,
            TextAlign::CENTER,
            "--°C");
    }

    // Omschrijving
    it.print(
        CENTER_X,
        CONDITION_Y,
        subtitle_font,
        BLACK,
        TextAlign::CENTER,
        weather_condition_text(condition));

    // Wind (links)
    draw_icon(
        it,
        LEFT_X,
        DETAIL_ICON_Y,
        small_font,
        BLUE,
        icon::WIND);

    if (is_valid(wind_speed))
    {
        it.printf(
            LEFT_X,
            DETAIL_VALUE_Y,
            small_font,
            BLACK,
            TextAlign::CENTER,
            "%.0f km/h %s",
            wind_speed,
            wind_direction_text(wind_bearing));
    }
    else
    {
        it.print(
            LEFT_X,
            DETAIL_VALUE_Y,
            small_font,
            BLACK,
            TextAlign::CENTER,
            "--");
    }

    // Luchtvochtigheid (rechts)
    draw_icon(
        it,
        RIGHT_X,
        DETAIL_ICON_Y,
        small_font,
        BLUE,
        icon::WATER);

    if (is_valid(humidity))
    {
        it.printf(
            RIGHT_X,
            DETAIL_VALUE_Y,
            small_font,
            BLACK,
            TextAlign::CENTER,
            "%.0f%%",
            humidity);
    }
    else
    {
        it.print(
            RIGHT_X,
            DETAIL_VALUE_Y,
            small_font,
            BLACK,
            TextAlign::CENTER,
            "--");
    }
}

} // namespace ui
