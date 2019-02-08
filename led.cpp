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

void ShowNoGPSData(void)
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

void ShowGPSData(double lat, double lng)
{
    for (size_t ix = 0; ix < cstripRing; ix++)
        stripRing[ix] = 0;

    for (size_t ix = 4; ix <= 20; ix++)
    {
        stripRing[IxRing(ix)] = CRGB::Green;
    }
    for (size_t ix = 0; ix < 24; ix += 6)
    {
        stripRing[IxRing(ix)] = CRGB::Blue;
    }

    FastLED.show();
}


// Distance, in meters, from the Golden Stake aka The Man
//
double DmFromMan(double lat, double lng)
{
    return DmFromLatLng(lat, lng, latMan, lngMan);
}


double BearingFromMan(double lat, double lng)
{
    return BearingFromLatLng(latMan, lngMan, lat, lng);
}


double DmFromLatLng(double lat1, double lng1, double lat2, double lng2) 
{
  // returns distance in meters between two positions, both specified 
  // as signed decimal-degrees latitude and longitude. Uses great-circle 
  // distance computation for hypothetical sphere of radius 6372795 meters.
  // Because Earth is no exact sphere, rounding errors may be up to 0.5%.
  // Courtesy of Maarten Lamers
  // https://github.com/mikalhart/TinyGPS/blob/master/TinyGPS.cpp

  double delta = radians(lng1-lng2);
  double sdlong = sin(delta);
  double cdlong = cos(delta);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  double slat1 = sin(lat1);
  double clat1 = cos(lat1);
  double slat2 = sin(lat2);
  double clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong); 
  delta = sq(delta); 
  delta += sq(clat2 * sdlong); 
  delta = sqrt(delta); 
  double denom = (slat1 * slat2) + (clat1 * clat2 * cdlong); 
  delta = atan2(delta, denom); 
  return delta * 6372795; 
}

double BearingFromLatLng (double lat1, double lng1, double lat2, double lng2) 
{
  // returns course in degrees (North=0, West=270) from position 1 to position 2,
  // both specified as signed decimal-degrees latitude and longitude.
  // Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
  // Courtesy of Maarten Lamers
  double dlon = radians(lng2-lng1);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  double a1 = sin(dlon) * cos(lat2);
  double a2 = sin(lat1) * cos(lat2) * cos(dlon);
  a2 = cos(lat1) * sin(lat2) - a2;
  a2 = atan2(a1, a2);
  if (a2 < 0.0)
  {
    a2 += TWO_PI;
  }
  return degrees(a2);
}
