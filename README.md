# Compact Infoscreen Home Assistent

![platform](https://img.shields.io/badge/platform-ESP32-blue)
![framework](https://img.shields.io/badge/framework-ESPHome%20%2F%20ESP--IDF-orange)

🇬🇧 [English](#english) | 🇳🇱 [Nederlands](#nederlands)

---

## English

An ESPHome project for a round TFT display (GC9A01A, 240×240) that shows
four dashboards and talks to Home Assistant via the native API: a battery
status page, an analog station clock, the current weather, and the salt
level of a water softener. All pages rotate automatically every 10 seconds.

### Screens

| Page | Content |
|---|---|
| **Battery** | One ring (red/orange/yellow/green based on percentage), total percentage in the middle, individual percentages for battery 1 and 2 below it |
| **Clock** | Analog station clock (NS/SBB style) with a red second hand |
| **Weather** | Icon + temperature + description + wind + humidity, straight from Home Assistant |
| **Salt level** | Ring gauge with a water drop icon, shows "REFILL" below 5% |

The color thresholds for all percentage gauges are: **red** below 5%,
**orange** 5–20%, **yellow** 20–50%, **green** from 50% up.

### Requirements

- ESP32 (development board, e.g. `esp32dev`)
- Round TFT display with a GC9A01A controller, 240×240, SPI (`mipi_spi`)
- Home Assistant with the following kind of entities available:
  - Two percentage sensors for batteries
  - A percentage sensor for a water softener's salt level
  - A `weather.*` entity (for temperature, humidity, wind, and condition)

### Installation

1. Copy this entire project (folder including `includes/` and `fonts/`) into
   your ESPHome configuration directory.
2. Copy `secrets.yaml.example` to `secrets.yaml` and fill in your own wifi
   credentials and a **self-generated** API key (see below).
3. Update the settings listed in the table below (your entity IDs, and GPIO
   pins if needed).
4. Compile and flash:
   ```bash
   esphome run compact-infoscreen-ha.yaml
   ```
   (the first time over USB; after that OTA/wifi works too)
5. go to home assistant, devices and services and check if you need to add the ESP to home assistant. (info would not be displayed if you forget this part)

### Which values do you need to change?

This project is built around specific Home Assistant entities from the
original author. Search `compact-infoscreen-ha.yaml` for the lines below and
replace them with your own entity IDs.

#### Sensors (`sensor:` block)

| In the file | Replace with |
|---|---|
| `entity_id: sensor.gasebatterij_1_laadpercentage` | your first battery percentage sensor |
| `entity_id: sensor.plug_in_battery_laadpercentage` | your second battery percentage sensor |
| `entity_id: sensor.zoutlevelsensor_salt_level_percentage` | your salt level percentage sensor |
| `entity_id: weather.huis` *(4x, for temperature/humidity/wind)* | your own `weather.*` entity |

#### Text sensor (`text_sensor:` block)

| In the file | Replace with |
|---|---|
| `entity_id: weather.huis` | the same `weather.*` entity as above |

> The weather attributes (`temperature`, `humidity`, `wind_speed`,
> `wind_bearing`) are available by default on virtually any `weather.*`
> entity in Home Assistant, so this normally works right after updating the
> entity ID.

#### Wifi & API key (`secrets.yaml`)

| Key | Description |
|---|---|
| `wifi_ssid` | your wifi network name |
| `wifi_password` | your wifi network password |
| `api_encryption_key` | **generate your own**, see below |

Generate your own, unique API encryption key (never reuse a key from an
example):
```bash
python3 -c "import secrets,base64; print(base64.b64encode(secrets.token_bytes(32)).decode())"
```
Paste the result into `secrets.yaml` under `api_encryption_key`. After
flashing, Home Assistant will ask you to re-confirm the device (Settings →
Devices & Services → ESPHome) — it picks up the new key automatically, no
need to type it in by hand.

#### Hardware / wiring (`esp32:`, `spi:`, `display:` blocks)

If your board is wired differently than the original, update:

| Setting | Current value |
|---|---|
| `esp32.board` | `esp32dev` |
| `spi.clk_pin` | `GPIO18` |
| `spi.mosi_pin` | `GPIO17` |
| `display.cs_pin` | `GPIO15` |
| `display.dc_pin` | `GPIO16` |
| `display.reset_pin` | `GPIO14` |

`invert_colors: true` is enabled because this specific GC9A01A panel
otherwise renders colors swapped (black/white inverted). If colors look
wrong on your screen, set it to `false`.

### Web interface

The device has a built-in web server (`http://<device-ip>/`) with:
- A restart button
- All sensor values (batteries, salt level, weather)
- "Current page": which of the 4 dashboards is currently shown on the screen
- "Battery total": the average of both batteries

### Known limitations

- This only shows the **current** weather, no forecast — Home Assistant
  doesn't expose hourly forecasts as a simple sensor attribute by default
  anymore (that now goes through the `weather.get_forecasts` service), so
  it was deliberately left out.
- The MDI icon font (`fonts/materialdesignicons-webfont.ttf`) only includes
  the glyphs listed in `compact-infoscreen-ha.yaml` under
  `font: extras: glyphs:`, to keep the firmware compact. To add more icons,
  add the corresponding unicode codepoints (`\Uxxxxxxxx`) to that list.

### Project structure

```
compact-infoscreen-ha.yaml   ESPHome configuration (the "main" file)
secrets.yaml.example         Template for secrets.yaml (wifi + API key)
fonts/                       MDI icon font
includes/                    C++ headers with all drawing/layout logic
  colors.h                   Colors + percentage color thresholds
  layouts.h                  Screen dimensions, center point, page enum
  icons.h                    MDI icon codepoints
  helpers.h                  General helper functions (clamp, sanitize, ...)
  gauge_*.h                  Generic ring/arc/text drawing logic
  battery_page.h             Battery page
  clock_page.h                Clock page
  weather_page.h                 Weather page
  salt_page.h                       Salt level page
  pages.h                              Switches between the 4 pages
```

### License

Add your preferred license here (e.g. MIT).

---

## Nederlands

Een ESPHome-project voor een rond TFT-display (GC9A01A, 240×240) dat vier
dashboards toont en met Home Assistant praat via de native API: een
batterijstatus, een analoge stationsklok, het actuele weer, en het
zoutwaterniveau van een waterontharder. Alle pagina's wisselen automatisch
elke 10 seconden.

### Schermen

| Pagina | Inhoud |
|---|---|
| **Batterij** | Eén ring (rood/oranje/geel/groen o.b.v. percentage), totaal percentage in het midden, losse percentages van accu 1 en 2 eronder |
| **Klok** | Analoge stationsklok (NS/SBB-stijl) met rode secondewijzer |
| **Weer** | Icoon + temperatuur + omschrijving + wind + luchtvochtigheid, rechtstreeks uit Home Assistant |
| **Zoutniveau** | Ring-gauge met waterdruppel-icoon, toont "REFILL" onder de 5% |

De kleurdrempels voor alle percentage-gauges zijn: **rood** onder 5%,
**oranje** 5–20%, **geel** 20–50%, **groen** vanaf 50%.

### Benodigdheden

- ESP32 (development board, bijv. `esp32dev`)
- Rond TFT-display met GC9A01A-controller, 240×240, SPI (`mipi_spi`)
- Home Assistant met de volgende soort entiteiten beschikbaar:
  - Twee percentage-sensoren voor accu's
  - Een percentage-sensor voor het zoutniveau van een waterontharder
  - Een `weather.*`-entity (voor temperatuur, luchtvochtigheid, wind en
    conditie)

### Installatie

1. Kopieer dit hele project (map inclusief `includes/` en `fonts/`) naar je
   ESPHome-configuratiemap.
2. Kopieer `secrets.yaml.example` naar `secrets.yaml` en vul je eigen
   wifi-gegevens en een **zelf gegenereerde** API-sleutel in (zie hieronder).
3. Pas de instellingen aan die in de tabel hieronder staan (jouw entity-id's,
   eventueel je GPIO-pinnen).
4. Compileer en flash:
   ```bash
   esphome run compact-infoscreen-ha.yaml
   ```
   (de eerste keer via USB; daarna kan dat ook via OTA/wifi)
5. ga naar home assistant, Apparaten en diensten en controleer of je de ESP nog moet toevoegen aan Home Assistant. (info wordt niet geladen en getoond als je dit vergeet te doen (ERVERING!!!))

### Welke waardes moet je aanpassen?

Dit project is gebouwd rond specifieke Home Assistant-entiteiten van de
oorspronkelijke maker. Zoek in `compact-infoscreen-ha.yaml` naar onderstaande
regels en vervang ze door je eigen entity-id's.

#### Sensoren (`sensor:` blok)

| In het bestand | Vervang door |
|---|---|
| `entity_id: sensor.gasebatterij_1_laadpercentage` | jouw eerste batterij/accu-percentage-sensor |
| `entity_id: sensor.plug_in_battery_laadpercentage` | jouw tweede batterij/accu-percentage-sensor |
| `entity_id: sensor.zoutlevelsensor_salt_level_percentage` | jouw zoutniveau-percentage-sensor |
| `entity_id: weather.huis` *(4x, voor temperatuur/luchtvochtigheid/wind)* | jouw eigen `weather.*`-entity |

#### Tekst-sensor (`text_sensor:` blok)

| In het bestand | Vervang door |
|---|---|
| `entity_id: weather.huis` | dezelfde `weather.*`-entity als hierboven |

> De weer-attributen (`temperature`, `humidity`, `wind_speed`,
> `wind_bearing`) zijn standaard aanwezig op vrijwel elke `weather.*`-entity
> in Home Assistant, dus dit werkt normaliter direct na het aanpassen van de
> entity-id.

#### Wifi & API-sleutel (`secrets.yaml`)

| Sleutel | Omschrijving |
|---|---|
| `wifi_ssid` | naam van je wifi-netwerk |
| `wifi_password` | wachtwoord van je wifi-netwerk |
| `api_encryption_key` | **zelf genereren**, zie onder |

Genereer een eigen, unieke API-encryptiesleutel (gebruik nooit een sleutel
uit een voorbeeld):
```bash
python3 -c "import secrets,base64; print(base64.b64encode(secrets.token_bytes(32)).decode())"
```
Plak de uitkomst in `secrets.yaml` bij `api_encryption_key`. Na het flashen
zal Home Assistant vragen om het apparaat opnieuw te bevestigen (Instellingen
→ Apparaten & Diensten → ESPHome) — de sleutel wordt daarbij automatisch
opgepikt, je hoeft 'm niet handmatig in te typen.

#### Hardware / bedrading (`esp32:`, `spi:`, `display:` blokken)

Als jouw board anders bedraad is dan het origineel, pas dan aan:

| Instelling | Huidige waarde |
|---|---|
| `esp32.board` | `esp32dev` |
| `spi.clk_pin` | `GPIO18` |
| `spi.mosi_pin` | `GPIO17` |
| `display.cs_pin` | `GPIO15` |
| `display.dc_pin` | `GPIO16` |
| `display.reset_pin` | `GPIO14` |

`invert_colors: true` staat aan omdat dit specifieke GC9A01A-paneel de
kleuren anders omgewisseld toont (zwart/wit verwisseld). Zie je dat juist
verkeerd staan op jouw scherm, zet deze dan op `false`.

### Webinterface

Het apparaat heeft een ingebouwde webserver (`http://<apparaat-ip>/`) met:
- Een herstart-knop
- Alle sensorwaardes (accu's, zoutniveau, weer)
- "Huidige pagina": welk van de 4 dashboards nu op het scherm staat
- "Batterij totaal": het gemiddelde van beide accu's

### Bekende beperkingen

- Dit toont alleen het **huidige** weer, geen voorspelling — Home Assistant
  biedt uurvoorspellingen niet standaard als eenvoudige sensor-attribuut aan
  (dat loopt tegenwoordig via de `weather.get_forecasts`-service), dus dat is
  bewust weggelaten.
- De MDI-icoon-font (`fonts/materialdesignicons-webfont.ttf`) bevat alleen de
  glyphs die in `compact-infoscreen-ha.yaml` onder `font: extras: glyphs:`
  zijn opgegeven, om de firmware compact te houden. Wil je extra iconen
  toevoegen, voeg dan de bijbehorende unicode-codepoints (`\Uxxxxxxxx`) toe
  aan die lijst.

### Projectstructuur

```
compact-infoscreen-ha.yaml   ESPHome-configuratie (het "hoofdbestand")
secrets.yaml.example         Voorbeeld voor secrets.yaml (wifi + API-sleutel)
fonts/                       MDI-icoon-lettertype
includes/                    C++ headers met alle teken-/lay-outlogica
  colors.h                   Kleuren + percentage-kleurdrempels
  layouts.h                  Schermafmetingen, middelpunt, pagina-enum
  icons.h                    MDI-icoon-codepoints
  helpers.h                  Algemene hulpfuncties (clamp, sanitize, ...)
  gauge_*.h                  Generieke ring-/boog-/tekst-tekenlogica
  battery_page.h             Batterijpagina
  clock_page.h                Klokpagina
  weather_page.h                 Weerpagina
  salt_page.h                       Zoutniveau-pagina
  pages.h                              Schakelt tussen de 4 pagina's
```

### Licentie

Voeg hier je gewenste licentie toe (bijvoorbeeld MIT).
