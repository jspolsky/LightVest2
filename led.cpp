#include "LightVest2.h"
#include "utils.h"
#include "led.h"

CRGB stripRing[cstripRing];
CRGB stripMain[cstripMain];

void InitLEDs(void)
{
    FastLED.addLeds<NEOPIXEL, pnstripRing>(stripRing, cstripRing);
    FastLED.addLeds<NEOPIXEL, pnstripMain>(stripMain, cstripMain);

    for (int i = 0; i < cstripMain; i++)
        stripMain[i] = CRGB::Green;

    for (int i = 0; i < cstripRing; i++)
        stripRing[i] = CRGB::Orange;

    FastLED.setBrightness(15);
    FastLED.show();
}

void ShowAudioLevel(byte scaled, byte scaledPeak)
{
    for (size_t ix = 0; ix < 16; ix++) 
    {
        if (ix < scaled)
        {
            stripMain[IxFromScaledLeft(ix)] = 
            stripMain[IxFromScaledRight(ix)] = CRGB::Blue;
        }
        else
        {
            stripMain[IxFromScaledLeft(ix)] = 
            stripMain[IxFromScaledRight(ix)] = 0;
        }
        
        if (ix == scaledPeak)
        {
            stripMain[IxFromScaledLeft(ix)] = 
            stripMain[IxFromScaledRight(ix)] = 0xFFFFFF;
        }
    }

    FastLED.show();
   
}