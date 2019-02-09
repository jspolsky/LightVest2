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

    FastLED.setBrightness(brightInit);

    ShowWayfinder();
}

void ShowWayfinder(void)
{
    for (int i = 0; i < cstripMain; i++)
        stripMain[i] = CHSV(hueWayfinder, 255, 255);

    FastLED.show();
}

void InitFire(void)
{
    for (int i = 0; i < cstripMain; i++)
        stripMain[i] = CRGB::Red;

    FastLED.show();
}

void ShowFlashlight(void)
{
    for (int i = 0; i < cstripVU; i++)
        stripMain[IxFromScaledLeft(i)] = 
        stripMain[IxFromScaledRight(i)] = 0xFFFFFF;

    FastLED.show();
}

void ShowGPSOnly(void)
{
    for (int i = 0; i < cstripMain; i++)
        stripMain[i] = 0;

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

void SetVestFrontColor(byte ix, CRGB rgb)
{
    stripMain[IxFromScaledLeft(ix)] =
    stripMain[IxFromScaledRight(ix)] = rgb;
}

void SetVestFrontColorDbl(byte ix, CRGB rgb)
{
    if (ix & 1)
        stripMain[IxFromScaledLeft(ix >> 1)] = rgb;
    else
        stripMain[IxFromScaledRight(ix >> 1)] = rgb;    
}

void ShowAudioLevel(byte scaled, byte scaledPeak)
{
    for (size_t ix = 0; ix < 16; ix++) 
    {
        if (ix < scaled)
        {
            SetVestFrontColor(ix, CHSV(11*(16-ix), 192, 255));
        }
        else
        {
            SetVestFrontColor(ix, 0);
        }
        
        if (ix == scaledPeak)
        {
            SetVestFrontColor(ix, CHSV(11*(16-ix), 255, 255));
        }
    }

    FastLED.show();
}

void IncreaseBrightness(void)
{
    uint8_t bright = FastLED.getBrightness();
    uint8_t dbright = (bright < 5) ? 1 : 3;    

    FastLED.setBrightness(min(brightMax, bright + dbright));
    FastLED.show();
}

void DecreaseBrightness(void)
{
    uint8_t bright = FastLED.getBrightness();
    uint8_t dbright = (bright < 5) ? 1 : 3;    

    FastLED.setBrightness(max(0, bright - dbright));
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
    double dmFromMan = DmFromMan(lat, lng);
    double bearingFromMan = BearingFromMan(lat, lng);
    double adj = ((bearingFromMan / 15.0) + 21.0);
    byte t = round(adj) % 24;

    if (dmFromMan < 200)
    {
        for (size_t ix = 0; ix < cstripRing; ix++)
            stripRing[ix] = (ix & 1) ? 0 : CRGB::Red;
    }
    else if (dmFromMan < 762)
    {
        for (size_t ix = 0; ix < cstripRing; ix++)
            stripRing[ix] = CRGB::Orange;
    }
    else
    {
        for (size_t ix = 0; ix < cstripRing; ix++)
            stripRing[ix] = 0;

        for (size_t ix = 4; ix <= 20; ix++)
        {
            stripRing[IxRing(ix)] = CRGB::Green;
        }
    }
    
    if (dmFromMan > 50)
    {    
        for (size_t ix = 0; ix < 24; ix += 6)
            stripRing[IxRing(ix)] = CRGB::Blue;
        stripRing[IxRing(t)] = CRGB::White;
    }

    FastLED.show();
}

void TestGPSData(double lat, double lng, const char* pszDescription)
{
    DebugPrintf("Test Location: %s\n", pszDescription);
    ShowGPSData(lat, lng);
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
