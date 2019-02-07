#include "LightVest2.h"
#include "utils.h"
#include "led.h"

CRGB stripRing[cstripRing];
CRGB stripMain[cstripMain];
byte hueWayfinder = 160;

void InitLEDs(void)
{
    FastLED.addLeds<NEOPIXEL, pnstripRing>(stripRing, cstripRing);
    FastLED.addLeds<NEOPIXEL, pnstripMain>(stripMain, cstripMain);

    for (int i = 0; i < cstripRing; i++)
        stripRing[i] = CRGB::Orange;

    FastLED.setBrightness(brightInc);

    ShowWayfinder();
}

void ShowWayfinder(void)
{
    for (int i = 0; i < cstripMain; i++)
        stripMain[i] = CHSV(hueWayfinder, 255, 255);

        FastLED.show();
}

void ShowFlashlight(void)
{
    for (int i = 0; i < cstripVU; i++)
        stripMain[IxFromScaledLeft(i)] = 
        stripMain[IxFromScaledRight(i)] = 0xFFFFFF;

    FastLED.show();
}

void IncreaseWayfinderHue(void)
{
    hueWayfinder = (hueWayfinder + 32) % 256;
    ShowWayfinder();
}

void DecreaseWayfinderHue(void)
{
    hueWayfinder = (hueWayfinder - 32) % 256;
    ShowWayfinder();
}

void ShowAudioLevel(byte scaled, byte scaledPeak)
{
    for (size_t ix = 0; ix < 16; ix++) 
    {
        if (ix < scaled)
        {
            stripMain[IxFromScaledLeft(ix)] = 
            stripMain[IxFromScaledRight(ix)] = CHSV(11*(16-ix), 192, 255);
        }
        else
        {
            stripMain[IxFromScaledLeft(ix)] = 
            stripMain[IxFromScaledRight(ix)] = 0;
        }
        
        if (ix == scaledPeak)
        {
            stripMain[IxFromScaledLeft(ix)] = 
            stripMain[IxFromScaledRight(ix)] = CHSV(11*(16-ix), 255, 255);
        }
    }

    FastLED.show();
   
}

void IncreaseBrightness(void)
{
    FastLED.setBrightness(min(brightMax, FastLED.getBrightness() + brightInc));
    FastLED.show();
}

void DecreaseBrightness(void)
{
    FastLED.setBrightness(max(0, FastLED.getBrightness() - brightInc));
    FastLED.show();
}

void ShowNoGPSData()
{
    static byte bWave = 0;

    EVERY_N_MILLISECONDS(10)
    {
        for (size_t ix = 0; ix < cstripRing; ix++)
            stripRing[ix] = 0;
        
        byte cube = scale8(cubicwave8(bWave), cstripRing);
        stripRing[IxRing(cube)] = CHSV(hueWayfinder, 255, 255);
        FastLED.show();

        bWave++;
    }
}
