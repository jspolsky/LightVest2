#include "LightVest2.h"
#include "utils.h"
#include "buttons.h"

/*
 * 
 * LED Light Vest 2.0
 * 
 * Requires:
 *      Adafruit Feather M0 Basic Prototype
 *      Five-way switch to control mode, color scheme, and brightness
 *      Adafruit Electret Microphone (https://www.adafruit.com/product/1063)
 *      Adafruit Ultimate GPS Featherwing (https://learn.adafruit.com/adafruit-ultimate-gps-featherwing)
 */

Adafruit_NeoPixel stripMain = Adafruit_NeoPixel(cstripMain, pnstripMain, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripRing = Adafruit_NeoPixel(cstripRing, pnstripRing, NEO_GRB + NEO_KHZ800);

void setup()
{
    Serial.begin(250000);

    pinMode(pnMode, INPUT_PULLDOWN);
    pinMode(pnUp, INPUT_PULLDOWN);
    pinMode(pnDown, INPUT_PULLDOWN);
    pinMode(pnPrev, INPUT_PULLDOWN);
    pinMode(pnNext, INPUT_PULLDOWN);

    while (!Serial && millis() < 2000L)
        ; // Wait for Serial port. If USB is disconnected timeout after 2 seconds

    float vBat = analogRead(pnBat) * 2.0 * 3.3 / 1024.0;
    DebugPrintf("READY. Battery %f volts.\n", vBat);

    stripMain.begin();
    stripMain.setBrightness(5);
    stripMain.show(); // Initialize all pixels to 'off'

    stripRing.begin();
    stripRing.setBrightness(5);
    stripRing.show(); // Initialize all pixels to 'off'

    COLOR c = stripMain.Color(128, 128, 0);

    for (int i = 0; i < cstripMain; i++)
        stripMain.setPixelColor(i, c);

    for (int i = 0; i < cstripRing; i++)
        stripRing.setPixelColor(i, c);

    stripMain.show();
    stripRing.show();

    InitButtons();
}

void loop()
{
    // let's look for buttons

    HandleButtonClicks(
        HIGH == digitalRead(pnMode),
        HIGH == digitalRead(pnUp),
        HIGH == digitalRead(pnDown),
        HIGH == digitalRead(pnPrev),
        HIGH == digitalRead(pnNext));

    // delay(100);
}
