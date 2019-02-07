#include "LightVest2.h"
#include "utils.h"
#include "buttons.h"
#include "gps.h"
#include "audio.h"
#include "led.h"

/*
 * 
 * LED Light Vest 2.0
 * 
 * Requires:
 *      Adafruit Feather M0 Basic Prototype
 *      Five-way switch to control mode, color scheme, and brightness
 *      Adafruit Electret Microphone (https://www.adafruit.com/product/1063)
 *      Adafruit Ultimate GPS Featherwing (https://learn.adafruit.com/adafruit-ultimate-gps-featherwing)
 *
 * Notes:
 *      Burning Man "Golden Spike" location https://innovate.burningman.org/datasets-page/
 *      4:30 is due south
 * 
 */



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

//    DebugPrintf("READY. Battery %f volts.\n", 
//                analogRead(pnBat) * 2.0 * 3.3 / 1024.0);

    InitButtons();
    InitGPS(Serial1);
    InitLEDs();
}

void loop()
{
    HandleButtonClicks(
        HIGH == digitalRead(pnMode),
        HIGH == digitalRead(pnUp),
        HIGH == digitalRead(pnDown),
        HIGH == digitalRead(pnPrev),
        HIGH == digitalRead(pnNext));

    ReadGPS(Serial1);
    ReadMic(analogRead(pnMic));
}

void message( message_t m )
{
    static byte scaledOld = 20;
    static byte scaledPeakOld = 20;

    switch (m)
    {
        case M_BUTTON_MODE:
            break;

        case M_BUTTON_UP:
            break;

        case M_BUTTON_DOWN:
            break;

        case M_BUTTON_PREV:
            break;

        case M_BUTTON_NEXT:
            break;

        case M_NEW_GPS_DATA:
            if (gpsdata.fFix)
            {
                DebugPrintf("GPS - %F,%F\n", gpsdata.lat.dec, gpsdata.lng.dec);
            }
            else
            {
                DebugPrintf("GPS - No data\n");
            } 
            break;

        case M_NEW_AUDIO_LEVEL:
            // if the level hasn't changed don't bother updating LEDs
            if (scaled != scaledOld || scaledPeak != scaledPeakOld)
            {
                ShowAudioLevel(scaled, scaledPeak);
                scaledOld = scaled;
                scaledPeakOld = scaledPeak;
            }
            
            break;
    }
}
