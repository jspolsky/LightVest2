#ifndef _LED_H_
#define _LED_H_


#define FASTLED_INTERNAL
#include <FastLED.h>

// PUBLIC:
void InitLEDs(void);
void IncreaseBrightness(void);
void DecreaseBrightness(void);
void ShowAudioLevel(byte scaled, byte scaledPeak);
void ShowWayfinder(void);
void ShowFlashlight(void);
void IncreaseWayfinderHue(void);
void DecreaseWayfinderHue(void);
void ShowNoGPSData(void);
void ShowGPSData(double lat, double lng);

// PRIVATE:
#define IxFromScaledLeft(scaledVal) ((scaledVal) + (cstripMain-cstripBack-cstripVU))
#define IxFromScaledRight(scaledVal) ((cstripVU-1) - (scaledVal))
#define IxRing(x) (((x) + 3) % cstripRing)

double DmFromMan(double lat, double lng);
double DmFromLatLng(double lat1, double long1, double lat2, double long2);
double BearingFromMan(double lat, double lng);
double BearingFromLatLng (double lat1, double lng1, double lat2, double lng2);

#endif
