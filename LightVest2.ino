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
 */

vestmode_t vestmode = VESTMODE_WAYFINDER;

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

    // DEBUG DEBUG
    // TEST SOME BEARING data

    DebugPrintf("")
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

    if (!gpsdata.fFix)
    {
        ShowNoGPSData();
    }
}

void message( message_t m )
{
    static byte scaledOld = 20;
    static byte scaledPeakOld = 20;

    switch (m)
    {
        case M_BUTTON_MODE:
            vestmode = (vestmode_t) ((vestmode + 1) % VESTMODE_FENCE);
            if (vestmode == VESTMODE_WAYFINDER)
                ShowWayfinder();
            else if (vestmode == VESTMODE_FLASHLIGHT)
                ShowFlashlight();
            break;

        case M_BUTTON_UP:
            IncreaseBrightness();
            break;

        case M_BUTTON_DOWN:
            DecreaseBrightness();
            break;

        case M_BUTTON_PREV:
            if (vestmode == VESTMODE_WAYFINDER)
                IncreaseWayfinderHue();
            break;

        case M_BUTTON_NEXT:
            if (vestmode == VESTMODE_WAYFINDER)
                DecreaseWayfinderHue();
            break;

        case M_NEW_GPS_DATA:
            if (gpsdata.fFix)
            {
                ShowGPSData(gpsdata.lat.dec, gpsdata.lng.dec); 
            }
            else
            {
                DebugPrintf("GPS - No data\n");
            }

            break;

        case M_NEW_AUDIO_LEVEL:
            if (vestmode != VESTMODE_VU)
                return;

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
