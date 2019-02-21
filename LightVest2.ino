#include "LightVest2.h"
#include "utils.h"
#include "buttons.h"
#include "gps.h"
#include "audio.h"
#include "led.h"
#include "fire.h"

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
bool fNearTheMan = false;

void setup()
{
    Serial.begin(250000);

    pinMode(pnMode, INPUT_PULLDOWN);
    pinMode(pnUp, INPUT_PULLDOWN);
    pinMode(pnDown, INPUT_PULLDOWN);
    pinMode(pnPrev, INPUT_PULLDOWN);
    pinMode(pnNext, INPUT_PULLDOWN);

//      while (!Serial && millis() < 2000L)
//          ; // Wait for Serial port. If USB is disconnected timeout after 2 seconds

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

    if (vestmode == VESTMODE_FIRE)
        ShowFire();

    if (vestmode == VESTMODE_METEOR)
        ShowMeteor();

    if (fNearTheMan)
        ShowNearTheManAnimation();

#ifndef TEST_GPS_MODE
    if (!gpsdata.fFix)
    {
        ShowNoGPSData();
    }
#endif 

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
            else if (vestmode == VESTMODE_FIRE)
                InitFire();
            else if (vestmode == VESTMODE_FLASHLIGHT)
            {
                ShowWayfinder();
                ShowFlashlight();
            }
            else if (vestmode == VESTMODE_GPS_ONLY)
                ShowGPSOnly();
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
#ifdef TEST_GPS_MODE

            switch (millis() / 3000 % 16)
            {
                case 1:
                    fNearTheMan = TestGPSData(40.780676, -119.213868, "Center Camp Cafe");
                    break;
                case 2:
                    fNearTheMan = TestGPSData(40.779929, -119.224457, "GTM");
                    break;
                case 3:
                    fNearTheMan = TestGPSData(40.791279, -119.212977, "9:00 portal");
                    break;
                case 4:
                    fNearTheMan = TestGPSData(40.781390, -119.199920, "3:00 portal");
                    break;
                case 5:
                    fNearTheMan = TestGPSData(40.791298, -119.200006, "The Temple");
                    break;
                case 6:
                    fNearTheMan = TestGPSData(40.800931, -119.211319, "The 747");
                    break;
                case 7:
                    fNearTheMan = TestGPSData(40.786390, -119.216042, "7:30, not esplanade");
                    break;
                case 8:
                    fNearTheMan = TestGPSData(40.786398, -119.215354, "7:30, esplanade");
                    break;
                case 9:
                    fNearTheMan = TestGPSData(40.793856, -119.209369, "Opulent Temple Inside");
                    break;
                case 10:
                    fNearTheMan = TestGPSData(40.793929, -119.208500, "Opulent Temple Outside");
                    break;
                case 11:
                    fNearTheMan = TestGPSData(40.793929, -119.208500, "Deep Playa");
                    break;
                case 12:
                    fNearTheMan = TestGPSData(40.783571, -119.193345, "Just Inside 2:00");
                    break;
                case 13:
                    fNearTheMan = TestGPSData(40.784067, -119.193281, "Just Outside 2:00");
                    break;
                default:
                    fNearTheMan = TestGPSData(40.786395, -119.206583, "The Man");
                    break;

            }    


#else
            if (gpsdata.fFix)
            {
                fNearTheMan = ShowGPSData(gpsdata.lat.dec, gpsdata.lng.dec); 
            }
#endif
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
